package macromedia.asc.util;

public class IntNumberConstant extends NumberConstant {

	private int ival;
	
	public IntNumberConstant(int num) {
		ival = num;
	}
	
	@Override
	public double doubleValue() {
		return ival;
	}

	@Override
	public Decimal128 decimalValue() {
		return new Decimal128(ival);
	}

	@Override
	public int intValue() {
		return ival;
	}

	@Override
	public long uintValue() {
		// AS3 just returns the bits
		return ival;
	}

	@Override
	public byte number_type() {
		return NumberUsage.use_int;
	}

	@Override
	public String toString() {
		return String.valueOf(ival);
	}

}
