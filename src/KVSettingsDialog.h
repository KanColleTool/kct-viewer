#ifndef KVSETTINGSDIALOG_H
#define KVSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

class QAbstractButton;
class KVMainWindow;
namespace Ui {
	class KVSettingsDialog;
}

class KVSettingsDialog : public QDialog {
	Q_OBJECT

public:
	explicit KVSettingsDialog(KVMainWindow *parent=0, Qt::WindowFlags f=0);
	virtual ~KVSettingsDialog();

public slots:
	virtual void accept();
	virtual void applySettings();

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);
	void on_proxyCheckbox_stateChanged(int state);
	void on_translationCheckbox_stateChanged(int state);
	void on_uploadScreenshotsCheckbox_stateChanged(int state);
	void on_browseButton_pressed();
	void on_openButton_pressed();

signals:
	void apply();

private:
	Ui::KVSettingsDialog *ui;
	QSettings settings;
};

#endif // KVSETTINGSDIALOG_H
