// ***************************************************
// * CS460: Programming Assignment 6: Test Program 1 *
// ***************************************************

function int sum_of_first_n_squares (int n)
{
  int sum;

  sum = 0;
  if (n >= 1)
  {
    sum = n * (n + 1) * (2 * n + 1) / 6;
  }
  return sum;
}
  
procedure main (void)
{
  int n;
  int sum;

  n = 100;
  sum = sum_of_first_n_squares (n);
  printf ("sum of the squares of the first %d numbers = %d\n", n, sum);
}
