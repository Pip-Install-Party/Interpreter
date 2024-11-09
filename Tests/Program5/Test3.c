/***************************************************
 * CS460: Programming Assignment 5: Test Program 3 *
 ***************************************************/

char announcement[2048];


procedure main (void)
{
  char name[100];

  name = 'Robert\x0';
  announcement = "You've got mail!\x0";
  display_announcement (name);
}


function bool empty_string (char string[4096])
{
  int i;
  int num_bytes_before_null;
  bool found_null;

  found_null = FALSE;
  num_bytes_before_null = 0;
  i = 0;
  while ((i < 4096) && (!found_null))
  {
    if (string[i] == '\x0')
    {
      found_null = TRUE;
    }
    else
    {
      num_bytes_before_null = num_bytes_before_null + 1;
    }
    i = i + 1;
  }
  return (num_bytes_before_null == 0);
}


procedure display_announcement (char name[512])
{
  if (!empty_string(name))
  {
    printf ("Welcome, %s\n\n", name);
    if (!empty_string(announcement))
    {
      printf ("%s\n", announcement);
    }
  }
}

