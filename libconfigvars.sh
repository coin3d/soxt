# libconfigvars.sh
# no to be executed - this script must be sourced

############################################################################
# Source Substitutions:

WIDGET="Widget"
EVENT="XAnyEvent *"
COMPONENTHEADER=""

# new
Gui=Xt
gui=xt

# old, to be removed
GUI="Xt"
lGUI="xt"
uGUI="XT"

export Gui gui GUI lGUI uGUI WIDGET EVENT COMPONENTHEADER

############################################################################
# Build Conditionals:

HAVE_RENDERAREA=true
HAVE_EXAMINERVIEWER=true
HAVE_PLANEVIEWER=true
HAVE_WALKVIEWER=true
HAVE_FLYVIEWER=true
HAVE_MATERIALLIST=true
HAVE_LIGHTSLIDERSET=true
HAVE_MATERIALSLIDERSET=true
HAVE_TRANSFORMSLIDERSET=true
HAVE_DIRECTIONALLIGHTEDITOR=false
HAVE_MATERIALEDITOR=false
HAVE_PRINTDIALOG=false

export HAVE_RENDERAREA HAVE_EXAMINERVIEWER
export HAVE_PLANEVIEWER HAVE_WALKVIEWER HAVE_FLYVIEWER
export HAVE_LIGHTSLIDERSET HAVE_MATERIALSLIDERSET HAVE_TRANSFORMSLIDERSET
export HAVE_DIRECTIONALLIGHTEDITOR HAVE_MATERIALEDITOR
export HAVE_MATERIALLIST HAVE_PRINTDIALOG

