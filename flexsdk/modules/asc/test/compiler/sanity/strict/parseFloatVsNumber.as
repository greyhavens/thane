// This tests the difference between Number(someString) and parseFloat(someString).  

print(Number(''));					// 0
print(Number('   '));				// 0
print(Number('   32'));				// 32
print(Number('   32   n'));			// NaN

print(parseFloat(''));				// NaN
print(parseFloat('   '));			// NaN
print(parseFloat('   32'));			// 32
print(parseFloat('   32   n'));		// 32
 