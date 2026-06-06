# Approximation of pi using the rectangle method
# Sequential version
#
# Pablo
# 2026-06-03

defmodule IntegralSeq do

  def compute_pi(n) do
    width = 1.0 / n

    sum =
      Enum.reduce(0..(n - 1), 0.0, fn i, acc ->
        mid = (i + 0.5) * width
        acc + 4.0 / (1.0 + mid * mid)
      end)

    width * sum
  end

  # Function to get the time taken by the function to run
  def measure_time(function, parameters) do
    {time, _result} = :timer.tc(function, parameters)
    time / 1_000_000
  end

end
