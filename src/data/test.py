import os

# file_src = r'src/data/all_word.txt'
# if os.path.exists(file_src):
# 	with open(file_src, 'r') as f:
# 		words = [w.replace(" ", "") for w in f.read().split('\n') if w != '']
# 		print(words[:100])
# else:
# 	print(f"File '{file_src}' does not exist.")

f = open('src/data/word_bank.csv', 'r')

i = 0
for line in f:
	words = line.split(',')
	word1, word2, w = words
	if word1 == 'autumn' and word2 == 'winter':
		print(w)
	if word1 == 'winter' and word2 == 'autumn':
		print(w)

