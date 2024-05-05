__author__ = 'Ori_Prince'

import os
import glob
import sys
from PyPDF2 import PdfMerger
import shutil


"pip install reportlab"
from reportlab.lib.pagesizes import A4  
from reportlab.pdfgen.canvas import Canvas  

FONTSIZE = 12
LINESIZE = 15
HEADINGSIZE = 16

CENTER = int(A4[0]/2)
MARGIN_LEFT = FONTSIZE
WIDTH = int(A4[0]) - MARGIN_LEFT


MERGED_FOLDER = r'c:\temp\merged_pdf'

HEADING_BEGIN = int(A4[1]) - HEADINGSIZE
PAGE_BEGIN = HEADING_BEGIN - HEADINGSIZE*2
LINES_IN_PAGE = int(PAGE_BEGIN / LINESIZE) + 1


def GetCodeByLines(filename):
    with open(filename, 'r') as f:
        return f.readlines()


def Heading(canvas, filename):
    canvas.setFont("Courier-Bold", HEADINGSIZE)
    canvas.drawString(CENTER-len(filename)/2*HEADINGSIZE , HEADING_BEGIN, f"{filename}")
     
     

def main(code_filename):
    #code_filename = input("Enter code filename >").strip()
    code = GetCodeByLines(code_filename.strip())
    
    i = 0
    while i < len(code):
        line_len = len(code[i])
        if line_len*(8) > WIDTH:
            new_line = []
            new_line_len = 0
            x = code[i].split(' ')
            while (line_len - new_line_len - (len(new_line)-1))*(8) > WIDTH:
                tmp = x.pop()
                new_line_len += len(tmp)
                new_line.insert(0,tmp)
            code[i] = ' '.join(x)
            j = 0
            while j < len(x) and x[j] == '':
                j += 1
            new_line = ' '*j + ' '.join(new_line)
            code.insert(i+1, new_line)
            i += 2
        else:
            i += 1
        
    
    pages = []
    s = 0
    e = LINES_IN_PAGE
    while s < len(code):
        pages.append(code[s:e])
        s = e
        e += LINES_IN_PAGE
    
    pdf_filename = ".".join((code_filename.split('.')[0] + "_" + code_filename.split('.')[1], "pdf"))
    
    canvas = Canvas(pdf_filename, pagesize = A4)
    
    for page in pages:
        Heading(canvas, code_filename)
        
        canvas.setFont("Courier", FONTSIZE)
        start = PAGE_BEGIN
        for line in page:
            canvas.drawString(MARGIN_LEFT, start, line[:-1]if line[-1] == '\n' else line)
            start -= LINESIZE
        canvas.showPage()
    
    canvas.save() 

def merge_pdfs(input_folder, output_path):
    merger = PdfMerger()

    input_files = glob.glob(f"{input_folder}/*.pdf")

    for path in input_files:
        merger.append(path)

    merger.write(output_path)
    merger.close()

if __name__ == '__main__':
    
    # part 1
    # main('server.py')
    # main('sql_orm.py')
    # main('server_address.txt')
    
    # main('main.cpp')
    # main('headers.hpp')
    
    # names = ['client', 'tools', 'toaster', 'player', 'object', 'map' ]
    
    # for name in names:
    #     main(name + '.cpp')
    #     main(name + '.hpp')
    
    # part 2
    merge_pdfs('pdfs', 'merged.pdf')
    
