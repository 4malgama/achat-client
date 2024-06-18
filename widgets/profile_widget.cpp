#include "profile_widget.h"
#include "ui_profile_widget.h"
#include "../network/account.h"
#include "../client.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>
#include <QListView>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QPainter>

namespace profilewidget
{
	ProfileWidget* w = nullptr;

	void close()
	{
		w = nullptr;
	}

	ProfileWidget* getInstance()
	{
		if (w == nullptr)
		{
			w = new ProfileWidget();
			QObject::connect(w, &ProfileWidget::event_close, close);
		}
		return w;
	}
}

namespace client { extern Client* window; }

static QImage circleImage(const QImage& image, QSize size)
{
	QImage output(size, QImage::Format_ARGB32);
	output.fill(Qt::transparent);

	QBrush brush(image.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	QPainter p(&output);
	p.setRenderHint(QPainter::Antialiasing);
	p.setPen(Qt::NoPen);
	p.setBrush(brush);
	p.drawEllipse(output.rect());
	return output;
}


namespace
{
	class PopupItemDelegate : public QStyledItemDelegate
	{
	public:
		using QStyledItemDelegate::QStyledItemDelegate;
		QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
		{
			QSize s = QStyledItemDelegate::sizeHint(option, index);
			s.setHeight(30);
			return s;
		}

		void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
		{
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);
			painter->setBrush(QBrush(QColor(0, 0, 0, 30)));
			painter->setPen(Qt::NoPen);
			painter->drawRect(option.rect);
			painter->restore();
			QStyledItemDelegate::paint(painter, option, index);
		}
	};
}

ProfileWidget::ProfileWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ProfileWidget)
{
	ui->setupUi(this);
	hide();

	QComboBox* cb_see_profile_photo = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_photo", tr("Who can see profile photo"), QVariant(0)).wgt;
	cb_see_profile_photo->addItem(tr("Everyone"));
	cb_see_profile_photo->addItem(tr("Friends"));
	cb_see_profile_photo->addItem(tr("Only me"));

	QComboBox* cb_see_profile_description = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_description", tr("Who can see profile description"), QVariant(0)).wgt;
	cb_see_profile_description->addItem(tr("Everyone"));
	cb_see_profile_description->addItem(tr("Friends"));
	cb_see_profile_description->addItem(tr("Only me"));

	QComboBox* cb_see_profile_comments = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_comments", tr("Who can see my profile comments"), QVariant(0)).wgt;
	cb_see_profile_comments->addItem(tr("Everyone"));
	cb_see_profile_comments->addItem(tr("Friends"));
	cb_see_profile_comments->addItem(tr("Only me"));

	QComboBox* cb_leave_comments = (QComboBox*) addOption(OPTION_COMBOBOX, "leave_comments", tr("Who can leave comments"), QVariant(0)).wgt;
	cb_leave_comments->addItem(tr("Everyone"));
	cb_leave_comments->addItem(tr("Friends"));
	cb_leave_comments->addItem(tr("Only me"));

	QComboBox* cb_see_profile_post = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_post", tr("Who can see my post"), QVariant(0)).wgt;
	cb_see_profile_post->addItem(tr("Everyone"));
	cb_see_profile_post->addItem(tr("Friends"));
	cb_see_profile_post->addItem(tr("Only me"));

	QComboBox* cb_send_friend_request = (QComboBox*) addOption(OPTION_COMBOBOX, "send_friend_request", tr("Who can send friend request"), QVariant(0)).wgt;
	cb_send_friend_request->addItem(tr("Everyone"));
	cb_send_friend_request->addItem(tr("Friends of friends"));
	cb_send_friend_request->addItem(tr("Nobody"));

	QComboBox* cb_see_online_status = (QComboBox*) addOption(OPTION_COMBOBOX, "see_online_status", tr("Who can see my online status"), QVariant(0)).wgt;
	cb_see_online_status->addItem(tr("Everyone"));
	cb_see_online_status->addItem(tr("Friends"));
	cb_see_online_status->addItem(tr("Only me"));

	QComboBox* cb_send_message = (QComboBox*) addOption(OPTION_COMBOBOX, "send_message", tr("Who can send message to me"), QVariant(0)).wgt;
	cb_send_message->addItem(tr("Everyone"));
	cb_send_message->addItem(tr("Friends"));
	cb_send_message->addItem(tr("Nobody"));

	QComboBox* cb_invite_to_groups = (QComboBox*) addOption(OPTION_COMBOBOX, "invite_to_groups", tr("Who can invite me to groups"), QVariant(0)).wgt;
	cb_invite_to_groups->addItem(tr("Everyone"));
	cb_invite_to_groups->addItem(tr("Friends"));
	cb_invite_to_groups->addItem(tr("Nobody"));

	QLineEdit* le_display_name = (QLineEdit*) addOption(OPTION_TEXTBOX, "display_name", tr("Display name"), QVariant("")).wgt;
	le_display_name->setPlaceholderText(tr("Leave it empty if you want to keep the same name"));

	updateData();

	connect(ui->btnUpdate, &QAbstractButton::clicked, this, [this] {
		QHash<QString, QVariant> changes = getChanges();
		updateData();
		if (changes.isEmpty()) return;
		client::window->acc->updateProfile(changes);
	});
}

ProfileWidget::~ProfileWidget()
{
	delete ui;
}

void ProfileWidget::closeEvent(QCloseEvent *)
{
	emit event_close();
	delete this;
}

ProfileWidget::Option& ProfileWidget::addOption(ProfileWidget::OptionView option, const QString& id, const QString& name, const QVariant& value)
{
	Option o;
	o.option = option;
	o.name = name;
	o.value = value;

	QLabel* lbl = new QLabel(this);
	lbl->setText(name);
	lbl->setWordWrap(true);

	QWidget* w = nullptr;
	switch (option)
	{
		case OPTION_CHECKBOX:
		{
			w = new QCheckBox(this);
			((QCheckBox*)w)->setChecked(value.toBool());
			break;
		}
		case OPTION_COMBOBOX:
		{
			w = new QComboBox(this);
			((QComboBox*)w)->view()->setItemDelegate(new PopupItemDelegate(w));
			((QComboBox*)w)->setCurrentIndex(value.toInt());
			break;
		}
		case OPTION_SLIDER:
		{
			w = new QSlider(this);
			((QSlider*)w)->setValue(value.toInt());
			break;
		}
		case OPTION_TEXTBOX:
		{
			w = new QLineEdit(this);
			((QLineEdit*)w)->setText(value.toString());
			break;
		}
	}
	w->setObjectName("option_" + name);
	w->setToolTip(description);

	o.wgt = w;

	QFormLayout* layout = (QFormLayout*) ui->settingsContent->layout();
	layout->addRow(lbl, w);

	options.insert(id, o);
	return options[id];
}

void ProfileWidget::setAvatar(const QImage& image)
{
	QPixmap pixmap = QPixmap::fromImage(circleImage(image, ui->avatar->size()));
	ui->avatar->setPixmap(pixmap);
}

void ProfileWidget::updateData()
{
	ui->btnProfileLink->setText(accountLink);
	ui->leFirstname->setText(name1);
	ui->leSurname->setText(name2);
	ui->lePatronymic->setText(name3);
	ui->lePost->setText(post);
	ui->textAboutMe->setPlainText(description);
}

QHash<QString, QVariant> ProfileWidget::getChanges()
{
	QHash<QString, QVariant> changes;

	if (ui->leFirstname->text() != name1) changes.insert("first_name", ui->leFirstname->text());
	if (ui->leSurname->text() != name2) changes.insert("sur_name", ui->leSurname->text());
	if (ui->lePatronymic->text() != name3) changes.insert("patronymic", ui->lePatronymic->text());
	if (ui->lePost->text() != post) changes.insert("post", ui->lePost->text());
	if (ui->textAboutMe->toPlainText() != description) changes.insert("description", ui->textAboutMe->toPlainText());

	return changes;
}
