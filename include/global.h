#ifndef GLOBAL_H
#define GLOBAL_H

#define DIAMETERINCH_MIN 10
#define DIAMETERINCH_MAX 31
#define CADENCE_MIN 50
#define CADENCE_MAX 200

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}TIME_CLOCK;

typedef struct
{
	TIME_CLOCK timeA;
	TIME_CLOCK timeR;
	uint16_t speed;
	uint16_t speedMax;
	uint16_t speedAvg;
	float distance;
	float odometer;
	uint8_t cadence;
	uint8_t cadenceLimit;
	uint8_t diameter;
}DATA_USER;

typedef enum
{
	BUTTON_NONE = 0,
	BUTTON_NEXT,
	BUTTON_CENTER,
	BUTTON_PREV
} BUTTON_STATUS;

extern DATA_USER user;
#endif