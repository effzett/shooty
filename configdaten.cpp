#include "configdaten.h"

ConfigDaten::ConfigDaten(QString version)
{
    m_version= version;
    ConfigDaten();
}

ConfigDaten::ConfigDaten()
{
    // Daten sind eigentlich überflüssig, da Defaultwerte bei QSettings angegeben wurden
    m_PositionX=35;
    m_PositionY=30;
    m_boxSize=130;

    m_b1x=35;
    m_b1y=170;
    m_b2x=35;
    m_b2y=205;
    m_b3x=35;
    m_b3y=230;

    m_drechteck         = true;
    m_esline            = true;
    m_poaks             = true;
    m_coiks             = true;
    m_poacoiline        = true;
    m_konvex            = true;
    m_trefferkoordinaten= false;

    m_rundKonvexCMR     = true;
    m_rundSpannung      = 0;
    m_rundKonvex        = true;
    m_rundPrecision     = 9;
    m_minPDFBox         =11;

    m_circleR           = false;
    m_circleSigma       = false;
    m_circleCEP50       = false;
    m_circleCEP90       = false;
    m_circleCEP95       = false;
    m_circleRinge       = false;

    m_regName="Testversion";
    m_regSerial="33293f853be99472";
}
