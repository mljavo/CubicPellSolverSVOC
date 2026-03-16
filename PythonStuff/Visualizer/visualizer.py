import json
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button
from mpl_toolkits.mplot3d import Axes3D

class PellVisualizer:
    def __init__(self, solutions_file, perf_file):
        self.solutions_file = solutions_file
        self.perf_file = perf_file
        self.pages = ["vykon", "kvadraticke", "kubicke"]
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

        # Tlačidlo pre prepínanie stránok
        ax_button = self.fig.add_axes([0.85, 0.94, 0.12, 0.04])
        current_page = self.pages[self.current_page_idx]
        self.btn = Button(ax_button, f"Ďalší: {self.pages[(self.current_page_idx+1)%3]}")
        self.btn.on_clicked(self.next_page)

        # ax_export = self.fig.add_axes([0.65, 0.92, 0.12, 0.05])
        # self.btn_export = Button(ax_export, "Uložiť PNG")
        # self.btn_export.on_clicked(self.save_current_page)

        if current_page == "vykon":
            self.ax = self.fig.add_subplot(111)
            plt.subplots_adjust(bottom=0.2, top=0.88, left=0.1, right=0.9)
            self.setup_annotation()
            self.draw_performance_page()
        elif current_page == "kubicke":
            self.ax = self.fig.add_subplot(111)
            plt.subplots_adjust(bottom=0.1, top=0.9, left=0.05, right=0.8)
            self.setup_annotation()
            self.draw_solutions_3d()
        elif current_page == "kvadraticke":
            self.ax = self.fig.add_subplot(111)
            plt.subplots_adjust(bottom=0.15, top=0.88, left=0.1, right=0.9)
            self.setup_annotation()
            self.draw_solutions_2d()

        self.fig.canvas.mpl_connect("motion_notify_event", self.hover)
        self.fig.canvas.draw_idle()

    def next_page(self, event):
        self.current_page_idx = (self.current_page_idx + 1) % 3
        self.draw_ui()

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


    def draw_solutions_2d(self):
        self.ax.set_title(r"Fundamentálne riešenia Pellovej rovnice $x^2 - Dy^2 = 1$", fontsize=14, pad=20)
        self.ax.grid(True, which="both", ls="-", alpha=0.5)
        
        best_sols = {}
        for r in [x for x in self.raw_results if int(x.get('degree', 0)) == 2]:
            d = int(r['d'])
            x_val = abs(int(r['coefficients'][0]))
            if d not in best_sols or x_val < abs(int(best_sols[d]['coefficients'][0])):
                best_sols[d] = r

        sorted_keys = sorted(best_sols.keys(), key=lambda d: abs(int(best_sols[d]['coefficients'][0])))

        for d in sorted_keys:
            r = best_sols[d]
            try:
                x_val = int(r['coefficients'][0])
                y_val = int(r['coefficients'][1])
                plot_limit = float(abs(x_val)) if len(str(abs(x_val))) < 15 else 1e15
                
                t = np.logspace(0, np.log10(plot_limit * 1.2), 100)
                y_hyp = np.sqrt(np.maximum(0, (t**2 - 1) / d))
                
                line, = self.ax.plot(t, y_hyp, alpha=0.3, label=f"D={d}")
                sc = self.ax.scatter(float(x_val), float(y_val), s=100, 
                                    color=line.get_color(), edgecolors='black', zorder=3)
                sc.set_gid(f"D: {d}\nx: {x_val}\ny: {y_val}")
                self.scatters.append(sc)
            except Exception as e:
                print(f"Preskočené D={d}: {e}")

        self.ax.set_xscale('log')
        self.ax.set_yscale('log')
        self.ax.set_xlabel("x (log)")
        self.ax.set_ylabel("y (log)")
        self.ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), 
                       ncol=min(len(sorted_keys), 6), frameon=True, fontsize=9)

    def draw_solutions_3d(self):
        self.ax.set_title(r"Fundamentálne riešenia kubickej Pellovej rovnice $x^3 + Dy^3 + D^2z^3 - 3Dxyz = 1$", fontsize=16, pad=30)
        self.ax.grid(True, which="both", ls="-", alpha=0.5)

        best_cubic_sols = {}
        for r in [x for x in self.raw_results if int(x.get('degree', 0)) == 3]:
            d = int(r['d'])
            x_val = abs(int(r['coefficients'][0]))
            if d not in best_cubic_sols or x_val < abs(int(best_cubic_sols[d]['coefficients'][0])):
                best_cubic_sols[d] = r

        sorted_sols = sorted(best_cubic_sols.values(), key=lambda r: abs(int(r['coefficients'][0])))
        cmap = plt.get_cmap('viridis')
        num_sols = len(sorted_sols)

        for i, r in enumerate(sorted_sols):
            c = r['coefficients']
            try:
                x_val = abs(float(int(c[0])))
                y_val = abs(float(int(c[1])))
                x_val = x_val if x_val > 0 else 0.1
                y_val = y_val if y_val > 0 else 0.1
                color = cmap(i / max(num_sols, 1))
                
                self.ax.plot([1, x_val], [1, y_val], color=color, alpha=0.15, lw=1, zorder=1)
                sc = self.ax.scatter(x_val, y_val, s=150, color=color, edgecolors='black', 
                                    zorder=3, label=f"D={r['d']}")
                sc.set_gid(f"D: {r['d']}\nx: {c[0]}\ny: {c[1]}\nz: {c[2]}")
                self.scatters.append(sc)
            except: continue

        self.ax.set_xscale('log'); self.ax.set_yscale('log')
        self.ax.set_xlabel("x (log)"); self.ax.set_ylabel("y (log)")
        
        if sorted_sols:
            self.ax.legend(loc='upper left', bbox_to_anchor=(1.05, 1), ncol=1, 
                           fontsize=9, frameon=True, title="Hodnoty D")
            
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

    # def save_current_page(self, event):
    #     page_name = self.pages[self.current_page_idx]
    #     filename = f"export_{page_name}.png"
    #     try:
    #         self.btn.ax.set_visible(False); self.btn_export.ax.set_visible(False)
    #         self.fig.savefig(filename, bbox_inches='tight', dpi=300)
    #         self.btn.ax.set_visible(True); self.btn_export.ax.set_visible(True)
    #         self.fig.canvas.draw_idle()
    #     except Exception as e: print(f"Chyba: {e}")

if __name__ == "__main__":
    import sys
    results_file = sys.argv[1] if len(sys.argv) > 1 else "results.json"
    perf_file = sys.argv[2] if len(sys.argv) > 2 else "perf_data.json"
    viz = PellVisualizer(results_file, perf_file)
    plt.show()