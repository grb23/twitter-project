#include <iostream>
#include <sstream>
#include <ctime>
#include "datetime.h"

DateTime::DateTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	hour = timeinfo->tm_hour, 
	min = timeinfo->tm_min, 
	sec = timeinfo->tm_sec, 
	year = timeinfo->tm_year+1900,
	month = timeinfo->tm_mon+1,
	day = timeinfo->tm_mday;
}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day)
{
	this->hour = hh;
	this->min = mm;
	this->sec = ss;
	this->year = year;
	this->month = month;
	this->day = day;
}

bool DateTime::operator<(const DateTime& other) const
{

	// check if they are exactly the same
	if(other.year == this->year &&
		other.month == this->month &&
		other.day == this->day &&
		other.hour == this->hour &&
		other.min == this->min &&
		other.sec == this->sec)
	{
		return false;
	}

	// else manually check everything
	if(this->year < other.year)
	{
		return true;
	}
	else if(this->year < other.year)
	{
		return false;
	}
	else if(this->month < other.month)
	{
		return true;
	}
	else if(this->month > other.month)
	{
		return false;		
	}
	else if(this->day < other.day)
	{
		return true;
	}
	else if(this->day > other.day)
	{
		return false;	
	}
	else if(this->hour < other.hour)
	{
		return true;
	}
	else if(this->hour > other.hour)
	{
		return false;
	}
	else if(this->min < other.min)
	{
		return true;
	}
	else if(this->min > other.min)
	{
		return false;
	}
	else if(this->sec < other.sec)
	{
		return true;
	}
	else if(this->sec > other.sec)
	{
		return false;
	}
	else
	{
		return false;	
	}

}

std::ostream& operator<<(std::ostream& os, const DateTime& other)
{
	if(other.year < 1000)
	{
		os << "0" << other.year << "-";
	}
	else
	{
		os << other.year << "-";
	}

	if(other.month < 10)
	{
		os << "0" << other.month << "-";
	}
	else
	{
		os << other.month << "-";
	}

	if(other.day < 10)
	{
		os << "0" << other.day << " ";
	}
	else
	{
		os << other.day << " ";
	}

	if(other.hour < 10)
	{
		os << "0" << other.hour << ":";
	}
	else
	{
		os << other.hour << ":";
	}

	if(other.min < 10)
	{
		os << "0" << other.min << ":";
	}
	else
	{
		os << other.min << ":";
	}

	if(other.sec < 10)
	{
		os << "0" << other.sec << " ";
	}
	else
	{
		os << other.sec << " ";
	}

	return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt)
{	
	std::string hold;
	std::string hold2;

	is >> hold;
	is >> hold2;
	std::string fullDate = hold + " " + hold2;

	// CHECK IF IT IS FORMATTED CORRECTLY
	bool fail = false;
	for(unsigned int i = 0; i < fullDate.length(); i++)
	{
		if((i >= 0 && i < 4) || 
			(i >= 5 && i < 7) ||
			(i >= 8 && i < 10) ||
			(i >= 11 && i < 13) ||
			(i >= 14 && i < 16) ||
			(i >= 17 && i < 19))
		{
			if(!isdigit(fullDate.at(i)))
			{
				//digit fail
				fail = true;
				break;
			}
	
		}
		else if(i == 4 || i == 7)
		{
			if(fullDate.at(i) != '-')
			{
				//dash fail
				fail = true;
				break;
			}
		}
		else if(i == 13 || i == 16)
		{
			if(fullDate.at(i) != ':')
			{
				//colon fail
				fail = true;
				break;
			}
		}
		else if(i == 10)
		{
			if(!isspace(fullDate.at(i)))
			{
				//space fail
				fail = true;
				break;
			}
		}
	}

	if(fail == true)
	{
		// return current time
		return is;
	}
	else
	{
		// stream in
		std::stringstream is2;
		for(unsigned int i = 0; i < fullDate.length(); i++)
		{
			if(fullDate.at(i) == ':')
			{
				fullDate[i] = ' ';
			}
			if(fullDate.at(i) == '-')
			{
				fullDate[i] = ' ';
			}	
		}
		is2 << fullDate;
		is2 >> dt.year >> dt.month >> dt.day >> dt.hour >> dt.min >> dt.sec;
		return is;
	}
	
	return is;
}
