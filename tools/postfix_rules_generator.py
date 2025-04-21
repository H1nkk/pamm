import itertools
import tkinter as tk
from tkinter import ttk, messagebox
import os

tokens = [
    "NONE", "FLOAT", "INT", "ID", "X", "Y", "Z", "W",
    "LPAR", "RPAR", "PLUS", "MINUS", "MULT", "CARET", "ASSIGN", "COMMA",
    "CALC", "DERX", "DERY", "DERZ", "DERW", "INTX", "INTY", "INTZ", "INTW",
    "INVALID", "ENDOFFILE"
]

class TokenMatrixApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Token Transition Matrix")
        
        self.check_vars = {}
        self.labels = {}
        frame = ttk.Frame(root)
        frame.pack(fill=tk.BOTH, expand=True)
        
        canvas = tk.Canvas(frame)
        v_scrollbar = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=canvas.yview)
        h_scrollbar = ttk.Scrollbar(root, orient=tk.HORIZONTAL, command=canvas.xview)
        self.scrollable_frame = ttk.Frame(canvas)
        
        self.scrollable_frame.bind(
            "<Configure>", lambda e: canvas.configure(
                scrollregion=canvas.bbox("all")
            )
        )
        
        canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        canvas.configure(yscrollcommand=v_scrollbar.set, xscrollcommand=h_scrollbar.set)
        
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        v_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        h_scrollbar.pack(side=tk.BOTTOM, fill=tk.X)
        
        header_label = ttk.Label(self.scrollable_frame, text="From ↓ / To →", borderwidth=1, relief="solid", width=12, anchor="center")
        header_label.grid(row=0, column=0, sticky="ew")
        
        for i, token1 in enumerate(tokens):
            lbl = ttk.Label(self.scrollable_frame, text=token1, borderwidth=1, relief="solid", width=10)
            lbl.grid(row=i+1, column=0, sticky="ew")
            lbl_col = ttk.Label(self.scrollable_frame, text=token1, borderwidth=1, relief="solid", width=2, anchor="s")
            lbl_col.grid(row=0, column=i+1, sticky="ns")
            lbl_col.bind("<Configure>", lambda e, lbl=lbl_col: lbl.configure(wraplength=1))
            self.labels[(None, tokens[i])] = lbl_col
            self.labels[(tokens[i], None)] = lbl
        
        for i, token1 in enumerate(tokens):
            for j, token2 in enumerate(tokens):
                var = tk.BooleanVar()
                chk = ttk.Checkbutton(self.scrollable_frame, variable=var)
                chk.grid(row=i+1, column=j+1)
                chk.bind("<Enter>", lambda e, r=token1, c=token2: self.highlight(r, c, True))
                chk.bind("<Leave>", lambda e, r=token1, c=token2: self.highlight(r, c, False))
                self.check_vars[(token1, token2)] = var
        
        load_btn = ttk.Button(root, text="Load", command=self.load_data)
        load_btn.pack(pady=5)
        save_btn = ttk.Button(root, text="Save", command=self.save_data)
        save_btn.pack(pady=5)
        
        self.load_data()
    
    def highlight(self, row, col, on):
        color = "lightblue" if on else "SystemButtonFace"
        if (row, None) in self.labels:
            self.labels[(row, None)].config(background=color)
        if (None, col) in self.labels:
            self.labels[(None, col)].config(background=color)
    
    def save_data(self):
        allowed_pairs = [
            f"{{ Lexer::TokenType::{t1}, Lexer::TokenType::{t2} }},"
            for (t1, t2), var in self.check_vars.items() if var.get()
        ]
        
        with open("allowed_pairs.txt", "w") as f:
            f.write("\n".join(allowed_pairs) + "\n")
        
        messagebox.showinfo("Saved", "Allowed pairs saved to allowed_pairs.txt")
    
    def load_data(self):
        if not os.path.exists("allowed_pairs.txt"):
            return
        
        with open("allowed_pairs.txt", "r") as f:
            lines = f.readlines()
        
        allowed_pairs = set()
        for line in lines:
            line = line.strip().strip("{},")
            parts = line.replace("Lexer::TokenType::", "").split(",")
            if len(parts) == 2:
                t1, t2 = parts[0].strip(), parts[1].strip()
                allowed_pairs.add((t1, t2))
        
        for (t1, t2), var in self.check_vars.items():
            var.set((t1, t2) in allowed_pairs)

if __name__ == "__main__":
    root = tk.Tk()
    app = TokenMatrixApp(root)
    root.mainloop()
