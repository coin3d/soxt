#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotationXYZ.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtMaterialEditor.h>
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
  SoSeparator * root;
  viewer->setSceneGraph(root = makescene());
  viewer->show();

  SoXtMaterialEditor * editor = new SoXtMaterialEditor;
  editor->attach(material);
  editor->show();

  SoXt::show(w);
  SoXt::mainLoop();
  return 0;
}

