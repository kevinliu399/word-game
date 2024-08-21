import gensim.downloader as api
import itertools
import csv
from gensim.models import KeyedVectors
from scipy.spatial.distance import cosine
import os

file_src = r'./src/data/all_word.txt'
if os.path.exists(file_src):
	with open(file_src, 'r') as f:
		words = [w.replace(" ", "") for w in f.read().split(',') if w != '']
		print(words[:100])
else:
	print(f"File '{file_src}' does not exist.")

# Load the Word2Vec model from gensim
model = api.load("word2vec-google-news-300")  # You can replace this with other models if needed

# Generate all possible pairs of words
word_pairs = list(itertools.combinations(words, 2))

# Function to calculate similarity
def calculate_similarity(word1, word2, model):
    if word1 in model and word2 in model:
        return 1 - cosine(model[word1], model[word2])
    else:
        return None  # Return None if one of the words is not in the model's vocabulary

# Generate similarity scores
data = []
for source, destination in word_pairs:
    similarity_score = calculate_similarity(source, destination, model)
    if similarity_score is not None:
        data.append((source, destination, round(similarity_score, 6)))

# Write to CSV
csv_file_path = "word_similarity.csv"
with open(csv_file_path, 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["source word", "destination word", "similarity score"])
    writer.writerows(data)

print(f"{csv_file_path} created!")
