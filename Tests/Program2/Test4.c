/***************************************************
 * CS460: Programming Assignment 2: Test Program 4 *
 ***************************************************/
procedure main (void)
{
  char buffer[256];
  bool is_palindrome;
  int i, num_bytes_read, half_num_bytes_read;

  buffer[255] = '\0';
  num_bytes_read = read (buffer, sizeof(buffer) - 2); // Note: buffer[255] reserved for a NULL string terminator
  for (i = 0; i < num_bytes_read; i = i + 1)
  {
    if (buffer[i] == '\n')
    {
      buffer[i] = '\0';
      if (i > 0)
      {
        num_bytes_read = i;
      }
      else
      {
        num_bytes_read = 0;
      }
    }
  }
  if (num_bytes_read >= 2)
  {
    half_num_bytes_read = (num_bytes_read - 1) / 2;
    i = 0;
    is_palindrome = TRUE;
    while ((i <= half_num_bytes_read) && is_palindrome)
    {
      is_palindrome = buffer[i] == buffer[num_bytes_read - 1 - i];
      i = i + 1;
    }
  }
  else
  {
    is_palindrome = FALSE;
  }
  if (num_bytes_read > 0)
  {
    if (is_palindrome)
    {
      printf ("%s is a palindrome\n", buffer);
    }
    else
    {
      printf ("%s is not a palindrome.\n", buffer);
    }
  }
}

