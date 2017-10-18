
class Elevator{
    private:
	    int floor_now;//当前楼层
		int next_target;
		bool up;  //true代表上楼
		bool down;
		bool status; //运行状态
		
    public:	
		
		Elevator()
		{
			
			this->floor_now = 3;
			this->up = true;
			this->down =false;
			this->status = false;
			
		}
		void setFloor(int floor)
		{
			this->floor_now = floor;
		}

		int getFloor()
		{
			return floor_now;
		}
		
		void setUp()
		{
			this->up = true;
			this->down = false;
		}
		void setDown()
		{
			this->up = false;
			this->down = true;
		}
		bool isUp()
		{
			return this->up;
		}
		bool isDown()
		{
			return this->down;
		}
		void Upstairs()
		{
			::Sleep(1000);
			this->floor_now++;
		}
		void Downstairs()
		{
			::Sleep(1000);
			this->floor_now--;
		}
		bool getStatus()
		{
			return this->status;
		}
		void run()
		{
			this->status = true;
		}
		void stop()
		{
			this->status = false;
		}


};


