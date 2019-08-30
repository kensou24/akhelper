#ifndef TESSERACT_H
#define TESSERACT_H

#include <QtCore>

class tesseract {
public:

  tesseract();
};

Q_GLOBAL_STATIC(tesseract,
                g_tesseract)
#endif // TESSERACT_H
