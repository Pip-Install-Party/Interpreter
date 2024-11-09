// ***************************************************
// * CS460: Programming Assignment 3: Test Program 9 *
// ***************************************************



// ***********************************************************************************
// * Hex digit converts a single character into its non-negative integer equivalent. *
// *                                                                                 *
// * Hex digit returns -1 upon error                                                 *
// ***********************************************************************************
function int hexdigit2int (char int) // syntax error - cannot use "int" for the name of a variable!
{
  int i, digit;

  digit = -1;
  if ((hex_digit >= '0') && (hex_digit <= '9'))
  {
    digit = hex_digit - '0';
  }
  else
  {
    if ((hex_digit >= 'a') && (hex_digit <= 'f'))
    {
      digit = hex_digit - 'a' + 10;
    }
    else
    {
      if ((hex_digit >= 'A') && (hex_digit <= 'F'))
      {
        digit = hex_digit - 'A' + 10; 
      }
    }
  }
  return digit;
}



procedure main (void)
{
  char hexnum[9];
  int i, digit, number; 

  number = 0;
  hexnum = "feed\x0";
  digit = 0;
  for (i = 0; (i < 4) && (digit > -1); i = i + 1)
  {
    digit = hexdigit2int (hexnum[i]);
    if (digit > -1)
    {
      number = number * 16 + digit;
    }
  }
  if (digit > -1)
  {
    printf ("Hex: 0x%s is %d decimal\n", hexnum, number);
  }
}

