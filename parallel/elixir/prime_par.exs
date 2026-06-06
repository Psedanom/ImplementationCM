# Sum of all prime numbers smaller than or equal to n
# Parallel version using tasks
#
# Pablo
# 2026-06-03

# Speedup measurements (fill in with your own runs):
# T1 (sequential):             11.556044 s
# Tp (p threads):  60 threads  2.209689 s
# Sp = T1 / Tp:                5.22971

defmodule PrimePar do

  # Check whether a number is prime
  def is_prime?(x) do
    cond do
      x < 2 -> false
      x == 2 -> true
      true -> not Enum.any?(2..ceil(:math.sqrt(x)),
                           fn i -> rem(x, i) == 0 end)
    end
  end

  # Sum the prime numbers within a range
  def sum_range({start, finish}) do
    start..finish
    |> Enum.filter(&is_prime?/1)
    |> Enum.sum()
  end

  # Sum all the prime numbers from 1 to n using multiple threads
  def parallel_sum_primes(n, t \\ System.schedulers) do
    step = ceil(n / t)
    starts = Enum.to_list(1..n//step)
    finishes = Enum.map(starts, fn s -> min(s + step - 1, n) end)

    Enum.zip(starts, finishes)
    |> Enum.map(&Task.async(fn -> sum_range(&1) end))
    |> Enum.map(&Task.await(&1, :infinity))
    |> Enum.sum()
  end

  # Function to get the time taken by the function to run
  def measure_time(function, parameters) do
    {time, _result} = :timer.tc(function, parameters)

    time / 1_000_000
  end

end
