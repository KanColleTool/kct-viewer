#ifndef KVSETTINGSDIALOG_H
#define KVSETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

class QAbstractButton;
class KVMainWindow;
namespace Ui {
	class KVSettingsDialog;
}

/**
 * Settings dialog.
 */
class KVSettingsDialog : public QDialog {
	Q_OBJECT

public:
	/**
	 * Constructor.
	 * 
	 * @param  parent Parent window
	 * @param  f      Window flags
	 */
	explicit KVSettingsDialog(KVMainWindow *parent=0, Qt::WindowFlags f=0);
	
	/**
	 * Destructor.
	 */
	virtual ~KVSettingsDialog();

public slots:
	/**
	 * Called when the OK button is clicked.
	 */
	virtual void accept();
	
	/**
	 * Write the current settings to storage.
	 * 
	 * \todo Rename this.
	 */
	virtual void applySettings();

private slots:
	void on_buttonBox_clicked(QAbstractButton *button);			///< \private
	void on_proxyCheckbox_stateChanged(int state);				///< \private
	void on_translationCheckbox_stateChanged(int state);		///< \private
	void on_uploadScreenshotsCheckbox_stateChanged(int state);	///< \private
	void on_browseButton_pressed();								///< \private
	void on_openButton_pressed();								///< \private

signals:
	/**
	 * Emitted when the OK button is clicked.
	 */
	void apply();

private:
	Ui::KVSettingsDialog *ui;
	QSettings settings;
};

#endif // KVSETTINGSDIALOG_H
