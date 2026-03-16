import json
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button

class PellVisualizer:
    def __init__(self, solutions_file, perf_file):
        self.solutions_file = solutions_file
        self.perf_file = perf_file
        self.pages = ["vykon"]
        self.current_page_idx = 0
        
        self.fig = plt.figure(figsize=(15, 9), dpi=100)
        self.annot = None
        self.scatters = []
        self.map_legend_to_points = {} 
        
        self.draw_ui()

    def load_json(self, filename):
        try:
            with open(filename, 'r', encoding='utf-8') as f:
                return json.load(f)
        except Exception as e:
            print(f"Warning: could not load JSON '{filename}': {e}")
            return None

    def draw_ui(self, event=None):
        self.fig.clear()
        self.raw_results = self.load_json(self.solutions_file) or []
        self.perf_data = self.load_json(self.perf_file) or []
        self.scatters = []
        self.map_legend_to_points = {}

        current_page = self.pages[self.current_page_idx]

        if current_page == "vykon":
            self.ax = self.fig.add_subplot(111)
            plt.subplots_adjust(bottom=0.2, top=0.88, left=0.1, right=0.9)
            self.setup_annotation()
            self.draw_performance_page()
        
        self.fig.canvas.mpl_connect("motion_notify_event", self.hover)
        self.fig.canvas.draw_idle()

    def draw_performance_page(self):      
        self.ax.set_title("Časová zložitosť algoritmov", fontsize=14, pad=20)
        self.ax.set_xlabel("Vstupné D (log)", fontsize=11)
        self.ax.set_ylabel("Čas výpočtu [µs] (log)", fontsize=11)
        self.ax.grid(True, which="both", ls="-", alpha=0.5)
        
        methods = {}
        for e in self.perf_data:
            m = e['method']
            display_name = "Cubic L-Form" if "Binary" in m or "binary" in m else m
            if display_name not in methods: methods[display_name] = {'d': [], 't': []}
            methods[display_name]['d'].append(int(e['d']))
            methods[display_name]['t'].append(e['time_us'])

        for name, vals in methods.items():
            d_vals, t_vals = np.array(vals['d']), np.array(vals['t'])
            valid = (d_vals > 0) & (t_vals > 0)
            if not any(valid): continue
            
            p = np.polyfit(np.log(d_vals[valid]), np.log(t_vals[valid]), 1)
            if "Brute" in name or "brute" in name:
                label_text = f"{name}: Vysoká variancia (závislosť od x0)"
            else:
                label_text = f"{name}: ~O(D^{p[0]:.2f})"
                
            sc = self.ax.scatter(d_vals, t_vals, s=80, edgecolors='black', label=label_text)
            sc.set_gid(f"Metóda: {name}\nD: {{x}}\nČas: {{y}} µs")
            self.scatters.append(sc)
            
            sort_idx = np.argsort(d_vals)
            self.ax.plot(d_vals[sort_idx], np.exp(p[1]) * d_vals[sort_idx]**p[0], '--', alpha=0.5)

        self.ax.set_xscale('log'); self.ax.set_yscale('log')
        self.ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.12), ncol=2, frameon=True)
            
    def setup_annotation(self):
        self.annot = self.ax.annotate("", xy=(0,0), xytext=(15,15),
            textcoords="offset points", 
            bbox=dict(boxstyle="round4,pad=0.5", fc="#F0F0F0", ec="#333333", alpha=0.95),
            arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=0"), zorder=100)
        self.annot.set_visible(False)

    def hover(self, event):
        if event.inaxes != self.ax: return
        found = False
        
        for sc in self.scatters:
            cont, ind = sc.contains(event)
            if cont:
                pos = sc.get_offsets()[ind["ind"][0]]
                self.annot.xy = pos
                
                gid = sc.get_gid()
                if gid:
                    if self.pages[self.current_page_idx] == "vykon":
                        val_x = f"{int(round(pos[0]))}"
                        if pos[1] < 100000:
                            val_y = f"{pos[1]:.2f}"
                        else:
                            val_y = f"{pos[1]:.2e}"
                            
                        text = gid.replace("{x}", val_x).replace("{y}", val_y)
                    else:
                        text = gid.replace("{x}", f"{pos[0]:.0f}").replace("{y}", f"{pos[1]:.0f}")
                    
                    self.annot.set_text(text)
                    self.annot.set_visible(True)
                    found = True
                    break
        
        if not found and self.annot.get_visible():
            self.annot.set_visible(False)
            self.fig.canvas.draw_idle()
        elif found:
            self.fig.canvas.draw_idle()

if __name__ == "__main__":
    import sys
    results_file = sys.argv[1] if len(sys.argv) > 1 else "results.json"
    perf_file = sys.argv[2] if len(sys.argv) > 2 else "perf_data.json"
    viz = PellVisualizer(results_file, perf_file)
    plt.show()