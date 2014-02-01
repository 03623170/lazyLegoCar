#include "observer.h"

using namespace std;


	Observer::Observer(rasp_sock::RaspberrySocket *sock,USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
		toArduino.x = 0;
		toArduino.y = 0;
		toArduino.pathFinding = false;
		angle = 90;
		incrementor = 1;
	}
	
	
	Observer::~Observer(){
	
	}

	void Observer::controlYellowLed(bool on)
	{
#ifdef RASP_DEBUG
		printf("Yellow LED\n");
#endif
		pipe->usbWrite(on?(void*)&CMD_YELLOW_LED_ON:(void*)&CMD_YELLOW_LED_OFF,sizeof(uint8_t));
	}

        void Observer::controlRedLed(bool on)
        {
#ifdef RASP_DEBUG
                printf("Red LED\n");
#endif
                pipe->usbWrite((on?((void*)&CMD_RED_LED_ON):((void*)&CMD_RED_LED_OFF)),sizeof(uint8_t));
        }


	void Observer::drive(uint8_t direction)
	{
#ifdef RASP_DEBUG
		printf("driving to: %d\n", direction);
#endif
		pipe->usbWrite((void*)&direction,sizeof(direction));
	}

	void Observer::steer(uint8_t direction)
	{
#ifdef RASP_DEBUG
		printf("steering to: %d\n", direction);
#endif
		pipe->usbWrite((void*)&direction,sizeof(direction));

		uint8_t mult = (direction==LEFT)?(-1):((direction==RIGHT)?1:0);
		moveFrontLaser(90-30*mult);
	}

	void Observer::moveFrontLaser(uint8_t degrees)
	{
#ifdef RASP_DEBUG
		printf("moving frontLaser: %d\n",degrees);
#endif
		pipe->usbWrite((void*)&CMD_MOVE_LASER_FRONT,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}

	void Observer::moveBackLaser(uint8_t degrees)
	{
#ifdef RASP_DEBUG
		printf("moving backLaser: %d\n", degrees);
#endif
		pipe->usbWrite((void*)&CMD_MOVE_LASER_BACK,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}
		
	void Observer::getFrontDistance(uint8_t* dist)
	{
#ifdef RASP_DEBUGG
		cout << "reading front dist" << endl;
#endif
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_FRONT,sizeof(uint8_t));
		pipe->usbRead(toArduino.comc.laserDataFront,LASERDATA_LEN,LASERDATA_LEN);
		if (dist != NULL){
			for(int i=0;i<LASERDATA_LEN;i++)
			{
				dist[i]=toArduino.comc.laserDataFront[i];
			}	
		}
	}

	uint8_t Observer::getBackDistance()
	{
#ifdef RASP_DEBUGG
		cout << "reading back dist" << endl;
#endif
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_BACK,sizeof(uint8_t));
		pipe->usbRead(&toArduino.comc.laserDataBack,sizeof(uint8_t));
		return toArduino.comc.laserDataBack;
	}
	
	
	void Observer::run(){
		std::string str;
		moveBackLaser(10);
		while(1){
			
			usleep(150*1000);
			//if(toArduino.pathFinding){
				printf("laser\n");
				angle+=incrementor*10;
				if(angle>=160 || angle <= 20)
					incrementor=-incrementor;
				moveBackLaser(angle);
			//}
		}
	}
