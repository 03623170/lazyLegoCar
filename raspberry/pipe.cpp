#include "pipe.h"

//namespace pipe{


	USBPipe::USBPipe(const std::string *pipe){
		if(pipe == NULL){
			this->linked = false;
			return;
		}
		this->fd = open (pipe->c_str(), O_RDWR | O_NOCTTY | O_SYNC);
		if(this->fd < 0){
			fprintf(stderr,"error %d opening %s %s\n", errno, pipe->c_str(), strerror(errno));
			this->linked = false;
		}
		else{
			
			this->linked = true;
		}
	}


	USBPipe::~USBPipe(){
		if(linked){
			close(fd);
			linked = false;
		}
	}


	void USBPipe::usbRead(uint8_t* buffer, size_t size){
		ZeroMemory(buffer, size);
		if(!linked)
			return;
		while(read(fd, buffer, size)==0)
			;//wait until data is available
	}
	
	
	int USBPipe::usbWrite(void *src, size_t size){
		if(!linked)
			return 0;
		return write(fd, src, size);
	}

	void USBPipe::usbFlush(){
		tcflush(fd,TCIOFLUSH);
	}

//};
