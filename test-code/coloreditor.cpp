#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtColorEditor.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

static SoMaterial * material;

SoSeparator *
makescene(void)
{
  SoSeparator * root = new SoSeparator;
  root->addChild(material = new SoMaterial);
  root->addChild(new SoCube);
  return root;
}

int
main(int argc, char ** argv)
{
  Widget w = SoXt::init(argc, argv, "SoXtColorEditor");
  SoXtExaminerViewer * viewer = new SoXtExaminerViewer(w);
  viewer->setSceneGraph(makescene());
  SoXtColorEditor * editor = new SoXtColorEditor;
  editor->attach(&(material->diffuseColor));
  // editor->setCurrentSliders(SoXtColorEditor::RGB_HSV);
  viewer->show();
  editor->show();
  SoXt::show(w);
  SoXt::mainLoop();
  return 0;
}

