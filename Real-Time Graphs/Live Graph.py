# https://medium.com/intel-student-ambassadors/live-graph-simulation-using-python-matplotlib-and-pandas-30ea4e50f883
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from itertools import count

plt.style.use("seaborn-v0_8-dark")
# df = pd.DataFrame();
# df['Values'] = []
# df['MVA'] = []
# df['t'] = []
x = []
y = []
mva = []
t = count()


def animate(i):
    i = next(t)
    x.append(i)
    y.append(i*i)

    if(len(x) > 10):
        mva.append(sum(y[-10:])/10)
    else:
        mva.append(0)
        
    if(len(x) > 50):
        x.pop(0)
        y.pop(0)
        mva.pop(0)

    plt.cla()  # clears the values of the graph
    plt.plot(x, y, linestyle='solid', label ='Values')
    plt.plot(x, mva, linestyle='solid', label ='MVA')

    plt.legend(loc='upper right')
    plt.xlabel('Time (ms)')
    plt.ylabel('Distance (cm)')
    plt.title('Sensor Data')
    plt.tight_layout()


ani = FuncAnimation(plt.gcf(), animate, 50)
plt.tight_layout()
plt.show()
