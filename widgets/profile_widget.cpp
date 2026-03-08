#include "profile_widget.h"
#include "ui_profile_widget.h"
#include "profile_avatar_widget.h"
#include "../network/account.h"
#include "../client.h"
#include "../utils/image_utils.h"
#include "../resource_manager/resource_manager.h"

#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>
#include <QListView>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QPainter>
#include <QStandardPaths>
#include <QFileDialog>
#include <QBuffer>


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

namespace console { void writeLine(const QString&); }


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


	avatarWgt = new ProfileAvatarWidget(this);
	ui->verticalLayout->insertWidget(0, avatarWgt);

	hide();

	QComboBox* cb_see_profile_photo = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_photo", tr("Who can see profile photo"), QVariant(0)).wgt;
	cb_see_profile_photo->addItem(tr("Everyone"), "everyone");
	cb_see_profile_photo->addItem(tr("Friends"), "friends");
	cb_see_profile_photo->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_see_profile_description = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_description", tr("Who can see profile description"), QVariant(0)).wgt;
	cb_see_profile_description->addItem(tr("Everyone"), "everyone");
	cb_see_profile_description->addItem(tr("Friends"), "friends");
	cb_see_profile_description->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_see_profile_comments = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_comments", tr("Who can see my profile comments"), QVariant(0)).wgt;
	cb_see_profile_comments->addItem(tr("Everyone"), "everyone");
	cb_see_profile_comments->addItem(tr("Friends"), "friends");
	cb_see_profile_comments->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_leave_comments = (QComboBox*) addOption(OPTION_COMBOBOX, "leave_comments", tr("Who can leave comments"), QVariant(0)).wgt;
	cb_leave_comments->addItem(tr("Everyone"), "everyone");
	cb_leave_comments->addItem(tr("Friends"), "friends");
	cb_leave_comments->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_see_profile_post = (QComboBox*) addOption(OPTION_COMBOBOX, "see_profile_post", tr("Who can see my post"), QVariant(0)).wgt;
	cb_see_profile_post->addItem(tr("Everyone"), "everyone");
	cb_see_profile_post->addItem(tr("Friends"), "friends");
	cb_see_profile_post->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_send_friend_request = (QComboBox*) addOption(OPTION_COMBOBOX, "send_friend_request", tr("Who can send friend request"), QVariant(0)).wgt;
	cb_send_friend_request->addItem(tr("Everyone"), "everyone");
	cb_send_friend_request->addItem(tr("Friends of friends"));
	cb_send_friend_request->addItem(tr("Nobody"), "nobody");

	QComboBox* cb_see_online_status = (QComboBox*) addOption(OPTION_COMBOBOX, "see_online_status", tr("Who can see my online status"), QVariant(0)).wgt;
	cb_see_online_status->addItem(tr("Everyone"), "everyone");
	cb_see_online_status->addItem(tr("Friends"), "friends");
	cb_see_online_status->addItem(tr("Only me"), "onlyme");

	QComboBox* cb_send_message = (QComboBox*) addOption(OPTION_COMBOBOX, "send_message", tr("Who can send message to me"), QVariant(0)).wgt;
	cb_send_message->addItem(tr("Everyone"), "everyone");
	cb_send_message->addItem(tr("Friends"), "friends");
	cb_send_message->addItem(tr("Nobody"), "nobody");

	QComboBox* cb_invite_to_groups = (QComboBox*) addOption(OPTION_COMBOBOX, "invite_to_groups", tr("Who can invite me to groups"), QVariant(0)).wgt;
	cb_invite_to_groups->addItem(tr("Everyone"), "everyone");
	cb_invite_to_groups->addItem(tr("Friends"), "friends");
	cb_invite_to_groups->addItem(tr("Nobody"), "nobody");

	QLineEdit* le_display_name = (QLineEdit*) addOption(OPTION_TEXTBOX, "display_name", tr("Display name"), QVariant("")).wgt;
	le_display_name->setPlaceholderText(tr("Leave it empty if you want to keep the same name"));

	updateData();

	connect(ui->btnUpdate, &QAbstractButton::clicked, this, [this] {
		QHash<QString, QVariant> changes = getChanges();
		updateData();
		if (changes.isEmpty()) return;
		client::window->acc->updateProfile(changes);
	});

	connect(avatarWgt, &ProfileAvatarWidget::clicked, this, &ProfileWidget::avatarClicked);
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

void ProfileWidget::avatarClicked()
{
	QString imagePath = QFileDialog::getOpenFileName(this, tr("Select new image"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), "Images (*.jpg; *.jpeg)");

	if (imagePath.isEmpty())
		return;

	QImage newImage(imagePath);

	if (newImage.isNull())
		return;

	ResourceManager& rm = ResourceManager::instance();
	rm.setAvatar(newImage);

	avatarWgt->setImage(ImageUtils::CropImageToCircle(newImage));

	console::writeLine("New image: " + imagePath);

	QByteArray imageData;
	QBuffer buffer(&imageData);
	buffer.open(QIODevice::WriteOnly);
	newImage.save(&buffer, "JPG");

	client::window->acc->sendNewAvatar(imageData);

	console::writeLine("Avatar updated.");
}

void ProfileWidget::updateOption(Option &opt, const QVariant &value)
{
	opt.value = value;
	switch (opt.option)
	{
		case OPTION_CHECKBOX:
		{
			static_cast<QCheckBox*>(opt.wgt)->setChecked(value.toBool());
			break;
		}
		case OPTION_COMBOBOX:
		{
			QComboBox* cb = static_cast<QComboBox*>(opt.wgt);
			cb->setCurrentIndex(cb->findData(value.toString()));
			break;
		}
		case OPTION_SLIDER:
		{
			static_cast<QSlider*>(opt.wgt)->setValue(value.toInt());
			break;
		}
		case OPTION_TEXTBOX:
		{
			static_cast<QLineEdit*>(opt.wgt)->setText(value.toString());
			break;
		}
	}
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
	w->setObjectName("option_" + id);
	w->setToolTip(description);

	o.wgt = w;

	QFormLayout* layout = (QFormLayout*) ui->settingsContent->layout();
	layout->addRow(lbl, w);

	options.insert(id, o);
	return options[id];
}

void ProfileWidget::setAvatar(const QImage& image)
{
	if (avatarWgt == nullptr)
	{
		console::writeLine("Widget of avatar == nullptr");
		return;
	}
	avatarWgt->setImage(ImageUtils::CropImageToCircle(image));
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

void ProfileWidget::updatePrivacySettings(const QHash<QString, QVariant> &privacySettings)
{
	for (auto it = privacySettings.cbegin(); it != privacySettings.cend(); ++it)
	{
		if (options.contains(it.key()))
		{
			Option& opt = options[it.key()];
			updateOption(opt, it.value());
		}
	}
}

QHash<QString, QVariant> ProfileWidget::getChanges()
{
	QHash<QString, QVariant> changes;

	if (ui->leFirstname->text() != name1) changes.insert("first_name", ui->leFirstname->text());
	if (ui->leSurname->text() != name2) changes.insert("sur_name", ui->leSurname->text());
	if (ui->lePatronymic->text() != name3) changes.insert("patronymic", ui->lePatronymic->text());
	if (ui->lePost->text() != post) changes.insert("post", ui->lePost->text());
	if (ui->textAboutMe->toPlainText() != description) changes.insert("description", ui->textAboutMe->toPlainText());

	QHash<QString, QVariant> privacySettings;

	for (auto it = options.cbegin(); it != options.cend(); ++it)
	{
		switch (it->option)
		{
			case OPTION_TEXTBOX:
				privacySettings.insert(it.key(), static_cast<QLineEdit*>(it->wgt)->text());
				break;
			case OPTION_CHECKBOX:
				privacySettings.insert(it.key(), static_cast<QCheckBox*>(it->wgt)->isChecked());
				break;
			case OPTION_COMBOBOX:
				privacySettings.insert(it.key(), static_cast<QComboBox*>(it->wgt)->currentData());
				break;
			case OPTION_SLIDER:
				privacySettings.insert(it.key(), static_cast<QSlider*>(it->wgt)->value());
				break;
		}
	}

	changes.insert("privacy", privacySettings);

	return changes;
}
