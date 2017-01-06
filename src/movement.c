#include "main.h"

void controlDrive(int speed, direction dir)
{
    switch(dir)
    {
    case FORWARD:
        setMotor(LFDrive, speed);
        setMotor(LBDriveI, speed);
        setMotor(LBDriveO, speed);
        setMotor(RFDrive, speed);
        setMotor(RBDriveI, speed);
        setMotor(RBDriveO, speed);
        break;

    case BACKWARD:
        setMotor(LFDrive, -speed);
        setMotor(LBDriveI, -speed);
        setMotor(LBDriveO, -speed);
        setMotor(RFDrive, -speed);
        setMotor(RBDriveI, -speed);
        setMotor(RBDriveO, -speed);
        break;

    case LEFT_TURN:
        setMotor(LFDrive, -speed);
        setMotor(LBDriveI, -speed);
        setMotor(LBDriveO, -speed);
        setMotor(RFDrive, speed);
        setMotor(RBDriveI, speed);
        setMotor(RBDriveO, speed);
        break;

    case RIGHT_TURN:
        setMotor(LFDrive, speed);
        setMotor(LBDriveI, speed);
        setMotor(LBDriveO, speed);
        setMotor(RFDrive, -speed);
        setMotor(RBDriveI, -speed);
        setMotor(RBDriveO, -speed);
        break;

    case LEFT_TURN_WIDE:
        setMotor(LFDrive, 0);
        setMotor(LBDriveI, 0);
        setMotor(LBDriveO, 0);
        setMotor(RFDrive, speed);
        setMotor(RBDriveI, speed);
        setMotor(RBDriveO, speed);
        break;

    case RIGHT_TURN_WIDE:
        setMotor(LFDrive, speed);
        setMotor(LBDriveI, speed);
        setMotor(LBDriveO, speed);
        setMotor(RFDrive, 0);
        setMotor(RBDriveI, 0);
        setMotor(RBDriveO, 0);
        break;

    case LEFT_STRAFE:
        setMotor(LFDrive, -speed);
        setMotor(LBDriveI, speed);
        setMotor(LBDriveO, speed);
        setMotor(RFDrive, speed);
        setMotor(RBDriveI, -speed);
        setMotor(RBDriveO, -speed);
         break;

    case RIGHT_STRAFE:
        setMotor(LFDrive, speed);
        setMotor(LBDriveI, -speed);
        setMotor(LBDriveO, -speed);
        setMotor(RFDrive, -speed);
        setMotor(RBDriveI, speed);
        setMotor(RBDriveO, speed);
         break;

    case STOP:
        setMotor(LFDrive, 0);
        setMotor(LBDriveI, 0);
        setMotor(LBDriveO, 0);
        setMotor(RFDrive, 0);
        setMotor(RBDriveI, 0);
        setMotor(RBDriveO, 0);
        break;

    default:
        return;
        break;
    }
}
void controlDriveEnc(int speed, direction dir, double counts)
{
    encoderReset(encoderRight.shaftEncoder);
    encoderReset(encoderLeft.shaftEncoder);
    controlDrive(speed, dir);
    if(dir == LEFT_TURN_WIDE)
    {
        while(getSensor(encoderRight.parent) < counts)
        {
            printf("%f\n\r", getEncoder(encoderRight));
            delay(20);
        }
    }

    else if(dir == BACKWARD || dir == LEFT_TURN)
    {
        while(getSensor(encoderLeft.parent) > counts)
        {
            printf("%f\n\r", getEncoder(encoderLeft));
            delay(20);
        }
    }

    else
    {
        while(getSensor(encoderLeft.parent) < counts)
        {
            printf("%f\n\r", getEncoder(encoderLeft));
        delay(20);
        }
    }
    controlDrive(0, STOP);
}

void controlLift(int speed)
{
    setMotor(leftLift1, speed);
    setMotor(leftLift2, speed);
    setMotor(rightLift1, speed);
    setMotor(rightLift2, speed);
}

typedef struct controlLiftEncArgs
{
    int speed;
    double encValue;
}controlLiftEncArgs;

void controlLiftEncTask(void *funcArgs)
{
    controlLiftEncArgs* args = funcArgs;
    controlLift(args->speed);
    if(args->speed >= 0)
    {
        while(getSensor(armEnc.parent) < args->encValue)
        {
            printf("%f\n\r", getSensor(armEnc.parent));
            delay(20);
        }
    }

    else
    {
        while(getSensor(armEnc.parent) < args->encValue)
        {
            printf("%f\n\r", getSensor(armEnc.parent));
            delay(20);
        }
    }
    controlLift(0);
}

void controlLiftEnc(int speed, double encValue, bool waitForTaskEnd)
{
    controlLiftEncArgs args = {speed, encValue};

    TaskHandle liftControl =
        taskCreate(controlLiftEncTask,
                   TASK_DEFAULT_STACK_SIZE, &args, TASK_PRIORITY_DEFAULT);

    if(waitForTaskEnd)
    {
        while(taskGetState(liftControl) == TASK_RUNNING){delay(20);}
    }
}
