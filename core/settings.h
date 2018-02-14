#ifndef SETTINGS_H
#define SETTINGS_H


// RsaToolbox
#include <Definitions.h>
#include <General.h>
#include <GenericBus.h>


// Application settings
const QString APP_NAME = "R&S PAE";
const QString APP_VERSION = "0.6";
const QString MANUFACTURER_FOLDER = "Rohde-Schwarz";
const QString APP_FOLDER = "PAE";
const QString APP_DESCRIPTION = "Measure Power Added Efficiency/Drain Efficiency of a power amplifier";
const QString CONTACT_INFO = "<html><head/><body><p>Nick Lalic<br/>VNA Software Developer<br/>San Francisco, CA USA<br/>+1 424 200 2846<br/>nick.lalic@rsa.rohde-schwarz.com<br/><a href=\"http://vna.rs-us.net\"><span style=\"text-decoration: underline; color:#0000ff;\">https://vna.rs-us.net</span></a></p></body></html>";

// Connection settings
const unsigned int TIMEOUT_MS = 1000;
#ifdef DEBUG_MODE
    const RsaToolbox::ConnectionType CONNECTION_TYPE     = RsaToolbox::ConnectionType::VisaTcpSocketConnection;
    const QString                    INSTRUMENT_ADDRESS  = "127.0.0.1::5025";

    const QDir dataDir(SOURCE_DIR);
    const QString LOG_FILENAME = dataDir.filePath("DEBUG_LOGFILE.txt");
    const QString KEY_PATH = dataDir.filePath("keys");
#else
    const RsaToolbox::ConnectionType CONNECTION_TYPE     = RsaToolbox::ConnectionType::VisaTcpSocketConnection;
    const QString                    INSTRUMENT_ADDRESS  = "127.0.0.1::5025";

    const QDir dataDir(RsaToolbox::GetAppDataPath(MANUFACTURER_FOLDER, APP_FOLDER));
    const QString LOG_FILENAME = dataDir.filePath("R&S PAE Log.txt");
    const QString KEY_PATH = dataDir.filePath("settings");
#endif

// Keys
const QString MY_KEY = "KEY_NAME";


#endif // SETTINGS_H

