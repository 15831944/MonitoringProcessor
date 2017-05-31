#ifndef CALCULATOR_H
#define CALCULATOR_H
	
class Calculator
{
public:
	void setThresholdValue(float v){
		thrValue = v;
	}
	void dataInput(int time, float d); 
	virtual float calculate(){ return 0; };
	virtual float calculate_window(){ return 0; };
	virtual float calculate(int start, int end){ return 0; };
	int getMark(){ return mark; }
	void clear()
	{
		datacnt = 0;
		ctime = -1;
		ltime = 0;
		//thrValue = 10.0f;
		mark = 0;
		time200 = 0;
	}
protected:
	int mark;
	float thrValue;
	float data[300];
	int datacnt;
	int ctime,ltime;
	int time200;
	float olddata;
};

#endif