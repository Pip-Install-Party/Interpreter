// ***************************************************
// * CS460: Programming Assignment 5: Test Program 5 *
// ***************************************************



// *******************************************************************************************
// * The fizzbuzz procedure outputs one of the following responses:                          *
// *                                                                                         *
// * If counter is divisible by three without remainder, display "Fizz".                     *
// * If counter is divisible by five without remainder, display "Buzz".                      *                
// * If counter is divisible by both three and five without a remainder, display "Fizzbuzz". *
// * If counter is NOT divisible by three or five, display the counter.                      *
// *******************************************************************************************
procedure fizzbuzz (int counter)
{
  int state;

  state = 0;
  if ((counter % 3) == 0)
  {
    state = 1;
  }
  if ((counter % 5) == 0)
  {
    state = state * 2 + 2;
  }
  if (state == 1)
  {
    printf ("Fizz");
  }
  else
  {
    if (state == 2)
    {
      printf ("Buzz");
    }
    else
    {
      if (state == 4)
      {
        printf ("Fizzbuzz");
      }
      else
      {
        printf ("%d", counter);
      }
    }
  }
}





procedure main (void)
{
  int counter;

  counter = 1;
  while (counter <= 100)
  {
    fizzbuzz (counter);
    counter = counter + 1;
    if (counter <= 100)
    {
      printf (", ");
    }
    else
    {
      printf ("\n");
    }
  }
}


