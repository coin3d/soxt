
#include <Inventor/Xt/SoXt.h>
#include <Inventor/sensors/SoAlarmSensor.h>

// *************************************************************************

void cbfunc(void * ra, SoSensor * s)
{
  (void)fprintf(stdout, "callback @ %s\n",
    SbTime::getTimeOfDay().formatDate("%G-%m-%d %H:%M:%S").getString());

  // reschedule
  SoAlarmSensor * alarm = (SoAlarmSensor *)s;
  alarm->setTimeFromNow(SbTime(1.0));
  alarm->schedule();
}

// *************************************************************************

int
main(int argc, char ** argv )
{
  Widget window = SoXt::init( argv[0] );

  SoAlarmSensor * alarm = new SoAlarmSensor(cbfunc, NULL);
  alarm->setTimeFromNow(SbTime(1.0));
  alarm->schedule();

  SoXt::mainLoop();

  return 0;
} // main()

// *************************************************************************
