#include "KVSettingsDialog.h"
#include "ui_KVSettingsDialog.h"
#include "KVMainWindow.h"
#include "KVDefaults.h"

#include <QAbstractButton>
#include <QFileDialog>
#include <QMessageBox>

KVSettingsDialog::KVSettingsDialog(KVMainWindow *parent, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::KVSettingsDialog) {
	ui->setupUi(this);

	ui->translationCheckbox->setChecked(settings.value("viewerTranslation", kDefaultTranslation).toBool());
	ui->reportUntranslatedCheckbox->setChecked(settings.value("reportUntranslated", kDefaultReportUntranslated).toBool());
	ui->taskbarProgressCheckbox->setChecked(settings.value("taskbarProgress", kDefaultTaskbarProgress).toBool());
	ui->proxyCheckbox->setChecked(settings.value("proxy", kDefaultProxy).toBool());
	ui->proxyServerEdit->setText(settings.value("proxyServer", kDefaultProxyServer).toString());
	ui->proxyPortBox->setValue(settings.value("proxyPort", kDefaultProxyPort).toInt());
	ui->proxyUserEdit->setText(settings.value("proxyUser", kDefaultProxyUser).toString());
	ui->proxyPassEdit->setText(settings.value("proxyPass", kDefaultProxyPass).toString());
	ui->uploadScreenshotsCheckbox->setChecked(settings.value("uploadScreenshots", kDefaultUploadScreenshots).toBool());
	ui->screenshotsPathEdit->setText(settings.value("screenshotsPath", kDefaultScreenshotsPath).toString());
	ui->cookieHackcheckBox->setChecked(settings.value("cookieHack", kDefaultCookieHack).toBool());

	switch(settings.value("proxyType", kDefaultProxyType).toInt()) {
	default:
	case QNetworkProxy::Socks5Proxy:
		ui->socksProxyRadio->setChecked(true);
		break;
	case QNetworkProxy::HttpProxy:
		ui->httpProxyRadio->setChecked(true);
		break;
	}

#ifdef __APPLE__
	ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
#endif

#if !defined(Q_OS_WIN)
	ui->windowsOnlyContainer->hide();
#endif

	this->on_proxyCheckbox_stateChanged(ui->proxyCheckbox->checkState());
	this->on_translationCheckbox_stateChanged(ui->translationCheckbox->checkState());

	this->adjustSize();
	this->setFixedSize(this->size());
}

KVSettingsDialog::~KVSettingsDialog() {

}

void KVSettingsDialog::accept() {
	this->applySettings();

	QDialog::accept();
}

void KVSettingsDialog::applySettings() {
	settings.setValue("viewerTranslation", ui->translationCheckbox->isChecked());
	settings.setValue("reportUntranslated", ui->reportUntranslatedCheckbox->isChecked());
	settings.setValue("taskbarProgress", ui->taskbarProgressCheckbox->isChecked());
	settings.setValue("proxy", ui->proxyCheckbox->isChecked());
	settings.setValue("proxyServer", ui->proxyServerEdit->text());
	settings.setValue("proxyPort", ui->proxyPortBox->value());
	if(ui->socksProxyRadio->isChecked())
		settings.setValue("proxyType", QNetworkProxy::Socks5Proxy);
	else if(ui->httpProxyRadio->isChecked())
		settings.setValue("proxyType", QNetworkProxy::HttpProxy);
	settings.setValue("uploadScreenshots", ui->uploadScreenshotsCheckbox->isChecked());
	settings.setValue("screenshotsPath", ui->screenshotsPathEdit->text());
	settings.setValue("cookieHack", ui->cookieHackcheckBox->isChecked());

	settings.sync();

	emit apply();
}

void KVSettingsDialog::on_buttonBox_clicked(QAbstractButton *button) {
	if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole)
		applySettings();
}

void KVSettingsDialog::on_proxyCheckbox_stateChanged(int state) {
	ui->proxyContainer->setEnabled(state == Qt::Checked);
}

void KVSettingsDialog::on_translationCheckbox_stateChanged(int state) {
	ui->reportUntranslatedCheckbox->setEnabled(state == Qt::Checked);
}

void KVSettingsDialog::on_uploadScreenshotsCheckbox_stateChanged(int state)
{
	ui->screenshotsPathContainer->setEnabled(state != Qt::Checked);
}

void KVSettingsDialog::on_browseButton_pressed()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setDirectory(ui->screenshotsPathEdit->text());

	if (dialog.exec())
	{
		ui->screenshotsPathEdit->setText(dialog.directory().absolutePath());
	}
}
