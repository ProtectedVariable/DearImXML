import fileinput
import io

if __name__ == "__main__":
	f = open("enum.raw")
	for line in f.readlines():
		enum_entry = line.split("=")[0].strip()
		if not (enum_entry.startswith("//") or len(enum_entry) == 0 or enum_entry.startswith("{") or enum_entry.startswith("}")):
			if enum_entry.startswith("enum "):
				print("//"+enum_entry)
			else:
				print('{ "'+enum_entry+'", '+enum_entry+' },')
