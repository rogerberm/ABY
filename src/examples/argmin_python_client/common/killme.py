import _ABY
import numpy as np

q = np.random.randint(100, size=(128, 1), dtype=np.uint32)

print _ABY.argmin(q)
