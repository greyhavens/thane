package macromedia.asc.util;

public class DecimalNumberConstant extends NumberConstant {

	private Decimal128 val;
	
	public DecimalNumberConstant(Decimal128 num) {
		val = num;
	}
	
	@Override
	public Decimal128 decimalValue() {
		return val;
	}

	@Override
	public double doubleValue() {
		return val.doubleValue();
	}

	@Override
	public int intValue() {
		return val.intValue();
	}

	@Override
	public byte number_type() {
		return NumberUsage.use_decimal;
	}

	@Override
	public long uintValue() {
		long lval = val.longValue();
		int ival = (int)lval; // truncates to 32 bits
		return ival;
	}
	
	@Override
	public String toString() {
		return val.toString();
	}


}
