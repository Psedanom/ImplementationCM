# Sum of all prime numbers smaller than or equal to n
# Sequential version
#
# Pablo
# 2026-06-03

defmodule PrimeSeq do

  # Check whether a number is prime
  def is_prime?(x) do
    cond do
      x < 2 -> false
      x == 2 -> true
      true -> not Enum.any?(2..ceil(:math.sqrt(x)),
                             fn i -> rem(x, i) == 0 end)
    end
  end

  # Sum all the prime numbers from 1 to n
  def sum_primes(n) do
    1..n
    |> Enum.filter(&is_prime?/1)
    |> Enum.sum()
  end

  # Function to get the time taken by the function to run
  def measure_time(function, parameters) do
    {time, _result} = :timer.tc(function, parameters)

    time / 1_000_000
  end

end
