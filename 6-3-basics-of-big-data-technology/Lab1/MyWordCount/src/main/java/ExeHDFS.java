import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;

import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;

public class ExeHDFS {
    static String hdfsPath = "/";

    public static void testView() throws IOException, URISyntaxException, InterruptedException {
        System.out.println("View file:");
        Configuration conf = new Configuration();
        conf.set("dfs.client.use.datanode.hostname", "true");
        conf.set("fs.defaultFS", "hdfs://120.46.147.42:8020");
        FileSystem hdfs = FileSystem.get(new URI("hdfs://120.46.147.42"), conf, "root");
        Path path = new Path(hdfsPath);
        FileStatus[] list = hdfs.listStatus(path);
        if (list.length == 0) {
            System.out.println("HDFS is empty.");
        } else {
            for (FileStatus f : list) {
                System.out.printf("name: %s, folder: %s, size: %d\n", f.getPath(), f.isDirectory(), f.getLen());
            }
        }
    }

    public static void testUpload() throws IOException, URISyntaxException, InterruptedException {
        System.out.println("Upload file:");
        Configuration conf = new Configuration();
        conf.set("dfs.client.use.datanode.hostname", "true");
        conf.set("fs.defaultFS", "hdfs://120.46.147.42:8020");
        FileSystem hdfs = FileSystem.get(new URI("hdfs://120.46.147.42"), conf, "root");
        InputStream in = new FileInputStream("./upload.txt");
        OutputStream out = hdfs.create(new Path(hdfsPath + "upload_2019211397.txt"));
        IOUtils.copyBytes(in, out, conf);
        System.out.println("Upload successfully!");
    }

    public static void testCreate() throws Exception {
        System.out.println("Write file:");
        Configuration conf = new Configuration();
        conf.set("dfs.client.use.datanode.hostname", "true");
        conf.set("fs.defaultFS", "hdfs://120.46.147.42:8020");
        byte[] buff = "Hello world! My name is mzh, my student id is 2019211397.".getBytes();
        FileSystem hdfs = FileSystem.get(new URI("hdfs://120.46.147.42"), conf, "root");
        Path dst = new Path(hdfsPath + "mzh_2019211397.txt");
        FSDataOutputStream outputStream = null;
        try {
            outputStream = hdfs.create(dst);
            outputStream.write(buff, 0, buff.length);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
        FileStatus files[] = hdfs.listStatus(dst);
        for (FileStatus file : files) {
            System.out.println(file.getPath());
        }
    }

    public static void testDownload() throws IOException, URISyntaxException, InterruptedException {
        System.out.println("Download file:");
        Configuration conf = new Configuration();
        conf.set("dfs.client.use.datanode.hostname", "true");
        conf.set("fs.defaultFS", "hdfs://120.46.147.42:8020");
        FileSystem hdfs = FileSystem.get(new URI("hdfs://120.46.147.42"), conf, "root");
        InputStream in = hdfs.open(new Path(hdfsPath + "mzh_2019211397.txt"));
        OutputStream out = new FileOutputStream("./download_2019211397.txt");
        IOUtils.copyBytes(in, out, conf);
        System.out.println("Download successfully!");
    }

    public static void main(String[] args) throws Exception {
        System.setProperty("hadoop.home.dir", "/");
        testView();
        testUpload();
        testCreate();
        testDownload();
        testView();
    }
}
