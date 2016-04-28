
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

class Keyboard
{
	
public:
	Keyboard();
	~Keyboard();
	char getPressedKey();
	
private:
	char pressedKey;
};

#endif // KEYBOARD_H_
