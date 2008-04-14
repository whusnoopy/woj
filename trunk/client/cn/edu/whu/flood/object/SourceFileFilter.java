package cn.edu.whu.flood.object;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;

import javax.swing.filechooser.FileFilter;

public class SourceFileFilter extends FileFilter {

  public void addType(String s) {
    exts.add(s);
  }
  
  public boolean accept(File f) {
    if (f.isDirectory())
      return true;
    else if(f.isFile()) {
      Iterator iter = exts.iterator();
      while (iter.hasNext()) {
        if(f.getName().endsWith((String)iter.next()))
          return true;
      }
    }
    return false;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }
  
  private ArrayList exts = new ArrayList();
  private String description;
}
