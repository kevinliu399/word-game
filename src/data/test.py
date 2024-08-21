import os

file_src = r'./src/data/all_word.txt'
if os.path.exists(file_src):
	with open(file_src, 'r') as f:
		words = [w.replace(" ", "") for w in f.read().split(',') if w != '']
		print(words[:100])
else:
	print(f"File '{file_src}' does not exist.")