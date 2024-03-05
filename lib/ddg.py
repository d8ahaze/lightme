import sys
import os
mod_dir = "/usr/lib/python3.11/site-packages/"
# mod_dir = "/root/zxcvx/packs/lightme/lib/python3.11/site-packages/"
sys.path.append(os.path.abspath(mod_dir))
sys.path.append(os.path.abspath("/usr/lib/python311.zip"))
sys.path.append(os.path.abspath("/usr/lib/python3.11"))
sys.path.append(os.path.abspath("/usr/lib/python3.11/lib-dynload"))

from bs4 import BeautifulSoup

HUB = "root/zxcvx"
NAME = "lightme"
PATH_WEBPAGE = "/" + HUB + "/packs/" + NAME + "/data/webpage.html"
PATH_SEARCH = "/" + HUB + "/packs/" + NAME + "/data/search.html"

def get_title():
	with open(PATH_WEBPAGE, "r", encoding="utf-8") as infile:
		html_content = infile.read()
		soup = BeautifulSoup(html_content, 'html.parser')
		return soup.title.text

def get_links():
	with open(PATH_SEARCH, "r", encoding="utf-8") as infile:
		html_content = infile.read()
		soup = BeautifulSoup(html_content, 'html.parser')
		extracted_links = soup.find_all("a", class_="result__url");
		for i in extracted_links:
			print(i.text.strip() + "\n")
		cleaned_links = tuple(link.text.strip() for link in extracted_links)
		return cleaned_links

# for link in soup.find_all("a", class_="result__url"):
#	outfile.write(link.text.strip() + "\n")

# path = "samples/html.duckduckgo.com/search.html"
# with open(path, 'r', encoding="utf-8") as infile:
#         html_content = infile.read()

# soup = BeautifulSoup(html_content, 'html.parser')

# with open('links.txt', 'w', encoding="utf-8") as outfile:
#       for link in soup.find_all("a", class_="result__url"):
#                outfile.write(link.text.strip() + "\n")

# if __name__ == "__main__":
# 	get_links()
