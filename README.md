# interfacing-stm32f401re-with-ds3231-rtc-module

For debugging,
ds3231_t struct variable contains 4 members:

1.)seconds

2.)minutes

3.)hours

4.)period (AM or PM)

period = 0 for AM

period = 1 for PM

period = 2 (no AM or PM due to 24 hour format)

Watchdog timer is used to reset the system in case
there's a software failure e.g. I2C bus hanging due to a hardware glitch.

For more information on the code, you can get across to me through the following:

1.)rjxmath123@gmail.com

2.)https://www.linkedin.com/in/olaoluwa-raji-043a18155/
