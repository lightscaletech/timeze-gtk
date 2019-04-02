#include "timeze_app.h"

int main(int argc, char ** argv) {
    return g_application_run (G_APPLICATION (timeze_app_new()), argc, argv);
}
