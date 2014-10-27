// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <Flash.h>

RTC_DS1307 rtc;

  int dayNumb;
  int monthNumb;
  int led = 13;
  boolean lightStatus;
  
  int setTime;
  int setTime2;
  int setHour;
  int setMin;
  
  int riseTime;
  int riseTime2;
  int riseHour;
  int riseMin;
  
  
  int nowTime;
  int nowHour;
  int nowMin;
  
  int nowAbs;
  int setAbs;
  int riseAbs;
  
  int difHour;
  int difMin;
  int difRiseH;
  int difRiseM;
  
  char buffer[] = "0";

  
//CREATE LOCAL TABLE OF SUN DATA HERE: http://aa.usno.navy.mil/data/docs/RS_OneYear.php
//USE 2016 OR OTHER LEAP YEAR SO FEB. 29 HAS DATA
//ALL TIMES PACIFIC STANDARD TIME - DOES NOT ACCOUT FOR DAYLIGHT SAVINGS 

//SUNSET DATA FOR TIGARD, OR
FLASH_TABLE(int, sunSet, 31,
    {1638,1639,1640,1641,1642,1643,1644,1646,1647,1648,1649,1650,1651,1653,1654,1655,1657,1658,1659,1701,1702,1703,1705,1706,1707,1709,1710,1712,1713,1715,1716},
    {1717,1719,1720,1722,1723,1725,1726,1728,1729,1731,1732,1733,1735,1736,1738,1739,1741,1742,1744,1745,1746,1748,1749,1751,1752,1753,1755,1756,1758},
    {1759,1800,1802,1803,1804,1806,1807,1809,1810,1811,1813,1814,1815,1817,1818,1819,1821,1822,1823,1824,1826,1827,1828,1830,1831,1832,1834,1835,1836,1837,1839},
    {1840,1841,1843,1844,1845,1846,1848,1849,1850,1852,1853,1854,1855,1857,1858,1859,1901,1902,1903,1904,1906,1907,1908,1910,1911,1912,1913,1915,1916,1917},
    {1918,1920,1921,1922,1923,1925,1926,1927,1928,1930,1931,1932,1933,1934,1936,1937,1938,1939,1940,1941,1942,1943,1944,1945,1946,1947,1948,1949,1950,1951,1952},
    {1953,1954,1954,1955,1956,1957,1957,1958,1959,1959,2000,2000,2001,2001,2002,2002,2002,2003,2003,2003,2003,2003,2003,2004,2004,2004,2004,2003,2003,2003},
    {2003,2003,2002,2002,2002,2001,2001,2000,2000,1959,1959,1958,1957,1957,1956,1955,1954,1953,1953,1952,1951,1950,1949,1948,1947,1945,1944,1943,1942,1941,1939},
    {1938,1937,1935,1934,1933,1931,1930,1928,1927,1925,1924,1922,1921,1919,1917,1916,1914,1912,1911,1909,1907,1905,1904,1902,1900,1858,1857,1855,1853,1851,1849},
    {1848,1846,1844,1842,1840,1838,1836,1834,1832,1830,1829,1827,1825,1823,1821,1819,1817,1815,1813,1811,1809,1807,1805,1804,1802,1800,1758,1756,1754,1752},
    {1750,1748,1746,1745,1743,1741,1739,1737,1735,1733,1732,1730,1728,1726,1725,1723,1721,1719,1718,1716,1714,1713,1711,1709,1708,1706,1705,1703,1702,1700,1659},
    {1657,1656,1655,1653,1652,1651,1649,1648,1647,1646,1645,1644,1642,1641,1640,1639,1638,1638,1637,1636,1635,1634,1634,1633,1632,1632,1631,1631,1630,1630},
    {1629,1629,1629,1628,1628,1628,1628,1628,1628,1628,1628,1628,1628,1628,1629,1629,1629,1630,1630,1630,1631,1632,1632,1633,1633,1634,1635,1636,1636,1637,1638}
  );
  
//SUNRISE DATA FOR TIGARD, OR
FLASH_TABLE(int, sunRise, 31, 
    {751,751,751,751,751,751,750,750,750,750,749,749,748,748,747,747,746,745,745,744,743,742,742,741,740,739,738,737,736,735,733},
    {732,731,730,729,727,726,725,723,722,721,719,718,716,715,713,712,710,709,707,705,704,702,700,659,657,655,654,652,650},
    {648,646,645,643,641,639,637,636,634,632,630,628,626,624,622,621,619,617,615,613,611,609,607,605,603,602,600,558,556,554,552},
    {550,548,547,545,543,541,539,537,535,534,532,530,528,526,525,523,521,519,518,516,514,513,511,509,508,506,505,503,502,500},
    {459,457,456,454,453,451,450,449,447,446,445,444,442,441,440,439,438,437,436,435,434,433,432,431,430,430,429,428,428,427,426},
    {426,425,425,424,424,424,423,423,423,422,422,422,422,422,422,422,422,422,422,423,423,423,423,424,424,424,425,425,426,426},
    {427,428,428,429,430,430,431,432,433,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,453,454,455},
    {456,457,458,459,501,502,503,504,505,507,508,509,510,511,513,514,515,516,517,519,520,521,522,524,525,526,527,528,530,531,532},
    {533,535,536,537,538,539,541,542,543,544,545,547,548,549,550,552,553,554,555,556,558,559,600,601,603,604,605,606,608,609},
    {610,611,613,614,615,616,618,619,620,622,623,624,625,627,628,629,631,632,633,635,636,638,639,640,642,643,644,646,647,649,650},
    {651,653,654,656,657,658,700,701,702,704,705,707,708,709,711,712,713,715,716,718,719,720,721,723,724,725,726,728,729,730},
    {731,732,733,735,736,737,738,739,740,740,741,742,743,744,745,745,746,747,747,748,748,749,749,749,750,750,750,751,751,751,751}
  );
  

void setup () {
  Serial.begin(9600);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(16, LOW);
  digitalWrite(17, HIGH);
  pinMode(led, OUTPUT);
  
   
  #ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
 
  
  //Do this once so we know what loop to start in
    DateTime now = rtc.now();
    monthNumb = now.month() - 1;
    dayNumb = now.day() -1;
    setTime = sunSet[monthNumb][dayNumb]; //[M][D]
    riseTime = sunRise[monthNumb][dayNumb];
    setHour = setTime / 100;
    setMin = setTime - (setHour * 100);
    riseHour = riseTime / 100;
    riseMin = riseTime - (riseHour * 100);

 
}

void loop () {

//Fetch the current time
    DateTime now = rtc.now();
    monthNumb = now.month() - 1;
    dayNumb = now.day() -1;
    nowHour = now.hour();
    nowMin = now.minute();
    nowTime = (nowHour * 100) + nowMin;
    
//Print the current time  
    Serial.print("Current time: ");
    sprintf(buffer, "%04d", nowTime); 
    Serial.print(buffer); 
    Serial.println(" PST");
   
 //Calculate the absolute time and time difference between now and today's sunset
    nowAbs = (nowHour * 60) + nowMin;
    setAbs = (setHour * 60) + setMin; 
    riseAbs = (riseHour * 60) + riseMin;
    difHour = (setAbs - nowAbs) / 60; 
    difMin = ((setAbs - nowAbs) - (difHour * 60));
   
    
    
    
    
 if ( (nowAbs - setAbs) >= 0 )
   {  //IF SUNSET HAS ALREADY OCCURED TODAY
   
   // calculate a date which is 23:59:59 into the future (should always be sometime tomorrow)
    DateTime future (now.unixtime() + 24 * 3600L - 1);
    monthNumb = future.month() - 1;
    dayNumb = future.day() -1;
    
    setTime = (sunSet[monthNumb][dayNumb]); //[M][D]
    setHour = setTime / 100;
    setMin = setTime - (setHour * 100);

    //Serial.println("Today's sunset has already occured.");
    Serial.print("The next sunset will occur tomorrow at ");
    Serial.print(setTime); 
    Serial.print(" PST in ");
    Serial.print(difHour + 23); //23 Hours plus change in minutes: so not 24
    Serial.print(" hours and ");
    Serial.print(difMin + 60);
    Serial.println(" minutes");
    //Serial.println();
    

 
 
 }
      
   
   else{ 
     //IF SUNSET HAS YET TO OCCUR TODAY
     
     //Calculate time sunset will happen today
    setTime = (sunSet[monthNumb][dayNumb]); //[M][D]
    setHour = setTime / 100;
    setMin = setTime - (setHour * 100);

    Serial.print("The next sunset will occur today at: ");
    Serial.print(setTime); 
    Serial.print(" PST in ");      
    Serial.print(difHour);
    Serial.print(" hours and ");
    Serial.print(difMin);
    Serial.println(" minutes");
   // Serial.println();
}



 
if ( (nowAbs - riseAbs) >= 0 ) {
  
    difRiseH = (riseAbs - nowAbs) / 60;
    difRiseM = ((riseAbs - nowAbs) - (difRiseH * 60));
  
    //calculate a date which is 23:59:59 into the future (should always be sometime tomorrow)
    DateTime future (now.unixtime() + 24 * 3600L - 1);
    monthNumb = future.month() - 1;
    dayNumb = future.day() -1;
   
    riseTime = sunRise[monthNumb][dayNumb];
    riseHour = riseTime / 100;
    riseMin = riseTime - (riseHour * 100);
 
    Serial.print("The next sunrise will occur tomorrow at: ");
    sprintf(buffer, "%04d", riseTime); 
    Serial.print(buffer); 
    Serial.print(" PST in ");
    Serial.print(difRiseH + 23); //23 Hours plus change in minutes: so not 24
    Serial.print(" hours and ");
    Serial.print(difRiseM + 60);
    Serial.println(" minutes");
    Serial.println();


}


else {
  
    difRiseH = (riseAbs - nowAbs) / 60;
    difRiseM = ((riseAbs - nowAbs) - (difRiseH * 60));
  
    riseTime = sunRise[monthNumb][dayNumb];
    riseHour = riseTime / 100;
    riseMin = riseTime - (riseHour * 100);
 
    Serial.print("The next sunrise will occur today at: ");
    sprintf(buffer, "%04d", riseTime); 
    Serial.print(buffer); 
    Serial.print(" PST in ");
    Serial.print(difRiseH); 
    Serial.print(" hours and ");
    Serial.print(difRiseM);
    Serial.println(" minutes");
    Serial.println();
  
}


 //TURN ON BETWEEN 30 MINS BEFORE SUNSET AND 11PM (23 HOURS * 60 MINS = 1380)
    if ( (nowAbs - setAbs) >= -30 && (nowAbs) <= 1380 )
   {
     digitalWrite(led, HIGH);
   }
   
//TURN ON BETWEEN 6:30 AM (6.5 * 60 = 390) UNTIL 30 MINS AFTER SUNRISE
   else if ( (nowAbs - riseAbs) <= 30 && (nowAbs) >= 390 )
   {
      digitalWrite(led, HIGH);
   }
   
   
   else {
      digitalWrite(led, LOW); 
    }
   
  
    delay(5000);
    
    
    
}
