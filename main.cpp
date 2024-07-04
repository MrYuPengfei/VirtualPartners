#include "PetWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PetWindow petWindow;
    petWindow.show();

    return app.exec();
}
