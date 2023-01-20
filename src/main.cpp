#include "ofMain.h"
#include "application.h"

int main()
{
  // paramètres du contexte de rendu OpenGL
  ofGLFWWindowSettings windowSettings;

  // option de redimentionnement de la fenêtre d'affichage
  windowSettings.resizable = true;

  // sélection de la version de OpenGL
  windowSettings.setGLVersion(4, 6);

  // création de la fenêtre
  ofCreateWindow(windowSettings);

  Application* application = new Application();

  // démarrer l'exécution de l'application
  ofRunApp(application);

}
