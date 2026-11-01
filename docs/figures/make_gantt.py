import matplotlib; matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrowPatch
# (label, first week, last week, on critical path)   schedule taken from the project handout
T=[("1 Planning and management",1,1,True),
   ("2 Requirements engineering  [after 1]",1,2,True),
   ("3 System design  [after Gate 1]",2,3,True),
   ("4 Implementation  [after Gate 2]",4,5,True),
   ("5 Verification and validation  [after 4 starts]",5,6,True),
   ("6 Config. mgmt and QA  [with 4 and 5]",4,6,False),
   ("7 Deployment  [after 5]",6,6,True),
   ("8 Maintenance: CR1, v1.1  [after 7]",7,7,True),
   ("9 Completion: report, viva  [after 7]",7,7,True)]
M=[("Gate 1: SRS approved",2),("Gate 2: design approved",3),("Gate 3: V&V done\nrelease v1.0",6),("release v1.1\nviva",7)]
fig,ax=plt.subplots(figsize=(11,5.6),dpi=170)
n=len(T)
for i,(lab,s,e,crit) in enumerate(T):
    y=n-i
    ax.barh(y,e-s+1,left=s-1,height=0.5,color='#c0392b' if crit else '#2e75b6',edgecolor='black',linewidth=0.6)
ax.set_yticks([n-i for i in range(n)]); ax.set_yticklabels([t[0] for t in T],fontsize=8.5)
ax.set_xlim(0,7); ax.set_xticks([x+0.5 for x in range(7)]); ax.set_xticklabels([f"Week {w}" for w in range(1,8)],fontsize=9)
ax.set_xticks(range(0,8),minor=True); ax.grid(axis='x',which='minor',color='#bbb',lw=0.6)
ax.set_ylim(-1.6,n+1.4)
for j,(lab,w) in enumerate(M):
    ax.plot(w,-0.2,marker='D',color='black',ms=8,clip_on=False)
    ax.annotate(lab,(w,-0.2),xytext=(w-0.04,-0.95),fontsize=7.5,ha=['right','left','right','right'][j])
ax.text(0.02,n+0.9,"Red = critical path     Blue = off the critical path     Diamond = milestone",fontsize=8)
ax.set_title("Gantt chart: Moderated LMS project (7 weeks, Waterfall)",fontsize=11,fontweight='bold')
ax.spines[['top','right']].set_visible(False)
plt.tight_layout(); plt.savefig('gantt.png'); print('gantt ok')
