class Controller{
	public:
		Controller();
		~Controller();
		void init();
		void setSpeed(double);
		double getRefSpeed();
		double getError();
	private:
		double wsoll;
};