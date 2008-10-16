var s = "splitme";
var separator = new RegExp();

var split_array = s.split(separator);

for (var index = 0; index < split_array.length; index++)
{
    print (split_array[index]);
}

var input = "oneword";

print("Splitting on a string: [" + input.split(" ").join() + "]");
print("Splitting on a regexp: [" + input.split(/ /).join() + "]");

var s = "xb ";
print(s.split(/ /g));

print(s.split(/$/g));

print(s.split(/^/g));