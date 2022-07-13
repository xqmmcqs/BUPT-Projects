package org.mzh2019211397.hbase.inputSource;

import org.apache.hadoop.hbase.Cell;
import org.apache.hadoop.hbase.client.Result;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapper;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.Text;

import java.io.IOException;

public class MemberMapper extends TableMapper<Writable, Writable> {
    private Text k = new Text();
    private Text v = new Text();
    public static final String FIELD_COMMON_separator = "\u0001";

    @Override
    protected void setup(Context context) throws IOException, InterruptedException {
    }

    @Override
    protected void map(ImmutableBytesWritable row, Result columns, Context context)
            throws IOException, InterruptedException {
        String value = null;
        String rowkey = new String(row.get());

        byte[] columnFamily = null;
        byte[] columnQualifier = null;
        long ts = 0L;

        try {
            for (Cell cell : columns.listCells()) {
                value = Bytes.toStringBinary(cell.getValueArray());
                columnFamily = cell.getFamilyArray();
                columnQualifier = cell.getQualifierArray();
                ts = cell.getTimestamp();
                k.set(rowkey);
                v.set(Bytes.toString(columnFamily) + FIELD_COMMON_separator + Bytes.toString(columnQualifier)
                        + FIELD_COMMON_separator + value + FIELD_COMMON_separator + ts);
                context.write(k, v);
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.err.println("Error:" + e.getMessage() + ",Row:" + Bytes.toString(row.get()) + ",Value" + value);
        }
    }
}
