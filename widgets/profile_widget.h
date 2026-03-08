#ifndef PROFILE_WIDGET_H
#define PROFILE_WIDGET_H

#include <QWidget>
#include <QVariant>
#include <QHash>


namespace Ui {
	class ProfileWidget;
}

class ProfileAvatarWidget;

class ProfileWidget : public QWidget
{
	Q_OBJECT

public:
	enum OptionView
	{
		OPTION_CHECKBOX,
		OPTION_COMBOBOX,
		OPTION_SLIDER,
		OPTION_TEXTBOX
	};

	struct Option
	{
		OptionView option;
		QString name;
		QVariant value;
		QWidget* wgt;
	};

	explicit ProfileWidget(QWidget *parent = nullptr);
	~ProfileWidget();

	ProfileAvatarWidget* avatarWgt;
	QString accountLink;
	QString name1; //firstname
	QString name2; //surname
	QString name3; //patronymic
	QString post;
	QString description;

	//TODO list of comments

	QHash<QString, Option> options;

	Option& addOption(OptionView option, const QString& id, const QString& name, const QVariant& value);
	void setAvatar(const QImage& image);

	void updateData();
	void updatePrivacySettings(const QHash<QString, QVariant>& privacySettings);

	QHash<QString, QVariant> getChanges();

signals:
	void event_close();

private:
	Ui::ProfileWidget *ui;
	void closeEvent(QCloseEvent *event);
	void avatarClicked();
	void updateOption(Option& opt, const QVariant& value);
};

#endif // PROFILE_WIDGET_H
