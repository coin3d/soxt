#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotationXYZ.h>

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
  SoSeparator * root;
  viewer->setSceneGraph(root = makescene());
  SoXtColorEditor * editor = new SoXtColorEditor;
  editor->attach(&(material->diffuseColor));
  viewer->show();
  editor->show();

  SoSeparator * editorscene = new SoSeparator;
  SoTranslation * trans = new SoTranslation;
  trans->translation.setValue(SbVec3f(2.0f, 0.0f, 0.0f));
  SoRotationXYZ * rot = new SoRotationXYZ;
  SoMaterial * mat = new SoMaterial;
  mat->diffuseColor.setValue(0.8, 0.8, 0.8);
  rot->axis = SoRotationXYZ::Y;
  rot->angle = 0.5;
  editorscene->addChild(trans);
  editorscene->addChild(rot);
  editorscene->addChild(mat);
  editorscene->addChild(editor->getSceneGraph());
  root->insertChild(editorscene, 0);

  SoXt::show(w);
  SoXt::mainLoop();
  return 0;
}

