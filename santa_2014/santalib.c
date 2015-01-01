// To compile: gcc santalib.c -shared -o libsanta.so
// To use in Python: 
//   compute_task = ctypes.cdll.LoadLibrary('./libsanta.so').compute_task
//   compute_task.retval = None

#include <math.h>

/* This function performs all computations necessary for doing a task.
 * Input arguments:
 *    duration         = duration of task (in minutes)
 *    starttime        = when to start (in minutes from 0:00 on 1 Jan)
 *    old_productivity = productivity of elf before starting the task
 * Output arguments:
 *    actual_duration  = actual duration, taking productivity into account (in minutes)
 *    elf_available    = when elf can start next job (in minutes from 0:00 on 1 Jan)
 *    new_productivity = productivity after task
 */
void compute_task(int duration, int starttime, double old_productivity,
		  int* actual_duration, int* elf_available, double* new_productivity)

{
  const int minsday = 24*60;
  const int sanctionedminsday = 10*60, unsanctionedminsday = 14*60;
  const int daystarts = 9*60, dayends = 19*60;
  int sanctioned, unsanctioned, wholedays, remainder_duration, day_starttime;
  int endtime, num_days_since_jan1, rest_time_in_working_days;
  int rest_time_remaining_minutes, local_start;

  /* compute actual duration */
  *actual_duration = (int) ceil((double) duration / old_productivity);

  /* split actual duration in sanctioned and unsanctioned part */
  wholedays = *actual_duration / minsday;
  sanctioned = wholedays * sanctionedminsday;

  remainder_duration = *actual_duration % minsday;
  day_starttime = starttime % minsday;
  endtime = day_starttime + remainder_duration;

  if (day_starttime < daystarts) {
    if (endtime <= daystarts) /* pass */ ;
    else if (endtime <= dayends) sanctioned += endtime - daystarts;
    else sanctioned += sanctionedminsday;
  } else if (day_starttime < dayends) {
    if (endtime <= dayends) sanctioned += remainder_duration;
    else if (endtime <= minsday + daystarts) sanctioned += dayends - day_starttime;
    else sanctioned += remainder_duration - unsanctionedminsday;
  } else {
    if (endtime <= minsday + daystarts) /* pass */ ;
    else if (endtime <= minsday + dayends) sanctioned += endtime - (minsday + daystarts);
    else sanctioned += sanctionedminsday;
  }

  unsanctioned = *actual_duration - sanctioned;
  
  /* compute new productivity */
  *new_productivity = pow(1.02, sanctioned / 60.0) * pow(0.9, unsanctioned / 60.0);
  //printf("%.16f\n", pow(1.02, sanctioned / 60.0));
  //printf("%.16f\n", pow(0.9, unsanctioned / 60.0));
  //printf("%.16f\n", *new_productivity);
  *new_productivity = *new_productivity * old_productivity;
  //printf("%.16f\n", *new_productivity);
  if (*new_productivity < 0.25) *new_productivity = 0.25;
  else if (*new_productivity > 4.0) *new_productivity = 4.0;

  /* compute when elf is again available, taking rest into account */
  *elf_available = starttime + *actual_duration;
  if (unsanctioned) {
    num_days_since_jan1 = *elf_available / minsday;
    rest_time_in_working_days = unsanctioned / sanctionedminsday;
    rest_time_remaining_minutes = unsanctioned % sanctionedminsday;
    local_start = *elf_available % minsday;
    if (local_start < daystarts) {
      local_start = daystarts;
    } else if (local_start > dayends) {
      num_days_since_jan1 += 1;
      local_start = daystarts; 
    };
    if (local_start + rest_time_remaining_minutes > dayends) {
      rest_time_in_working_days += 1;
      rest_time_remaining_minutes -= (dayends - local_start);
      local_start = daystarts;
    }
    *elf_available = (num_days_since_jan1 + rest_time_in_working_days) * minsday
      + local_start + rest_time_remaining_minutes;
  }
}
