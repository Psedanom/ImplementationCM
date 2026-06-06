# Approximation of pi using the rectangle method
# Parallel version using tasks
#
# Pablo
# 2026-06-03

# Speedup measurements:
# T1 (sequential):            2.22236 s
# Tp (p threads): 60 threads  0.305451 s
# Sp = T1 / Tp:               7.27566778

defmodule IntegralPar do

  # Receives a tuple and the width of the rectangles
  def sum_range({start, finish}, width) do
    Enum.reduce(start..finish, 0.0, fn i, acc ->
      mid = (i + 0.5) * width
      acc + 4.0 / (1.0 + mid * mid)
    end)
  end

  # Approximate pi by summing the area of n rectangles, using multiple threads
  def parallel_compute_pi(n, t \\ System.schedulers) do
    width = 1.0 / n
    step = ceil(n / t)
    starts = Enum.to_list(0..(n - 1)//step)
    finishes = Enum.map(starts, fn s -> min(s + step - 1, n - 1) end)

    sum =
      Enum.zip(starts, finishes)
      |> Enum.map(&Task.async(fn -> sum_range(&1, width) end))
      |> Enum.map(&Task.await(&1, :infinity))
      |> Enum.sum()

    width * sum
  end

  # Function to get the time taken by the function to run
  def measure_time(function, parameters) do
    {time, _result} = :timer.tc(function, parameters)
    time / 1_000_000
  end

end
