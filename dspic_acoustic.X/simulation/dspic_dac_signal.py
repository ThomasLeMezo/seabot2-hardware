
import matplotlib.pyplot as plt
import numpy as np

# Generate x values from 0 to 2*pi
signal_duration = 250e-3
dt = 1e-6
t = np.linspace(0, signal_duration, int(signal_duration/dt))
signal_fade = 10e-3
freq_middle = 40000
freq_range = 5000

# const double amplitude = dac_amplitude * fmin(1.0, t/signal_fade)
# * fmin(1.0, (signal_duration_ms - t)/signal_fade);
# double frequency = freq_middle+invert*freq_range*(t/signal_duration-0.5);
# data_chirp[j] = round(amplitude*sin(2.0*M_PI*t*frequency) + dac_mean);

amplitude = 0.5 * np.minimum(np.ones_like(t), t/signal_fade) * np.minimum(np.ones_like(t), (signal_duration-t)/signal_fade)
frequency = freq_middle + freq_range*(t/signal_duration-0.5)
frequency_phase = freq_middle + freq_range*(t/(2.0*signal_duration)-0.5)

data = amplitude * np.sin(2*np.pi*t*frequency)

# Compute the sine of x

# Create the plot
# plt.figure(figsize=(8, 4))
plt.plot(t, data, label='function(x)', color='blue')

plt.xlabel('time')
plt.ylabel('dac value')

# # Add grid and legend
# plt.grid(True, linestyle='--', alpha=0.7)
# plt.legend()

# Display the plot
plt.show()
