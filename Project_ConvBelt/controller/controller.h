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
		double wsoll_old;
};
extern "C"
{
	double getErrorC();
}
