require 'bitcache/spec'

share_as :Bitcache_List do
  include Bitcache::Spec::Matchers

  before :each do
    raise '+@class+ must be defined in a before(:all) block' unless instance_variable_get(:@class)
    @id0 = Bitcache::Identifier.new("\x00" * 16)
    @id1 = Bitcache::Identifier.new("\x01" * 16)
    @id2 = Bitcache::Identifier.new("\x02" * 16)
    @list = @class.new([@id0, @id1, @id2])
  end

  describe "List[]" do
    it "returns a List" do
      @class[@id0, @id1, @id2].should be_a List
    end
  end

  describe "List#clone" do
    it "returns a List" do
      @list.clone.should be_a List
    end

    it "returns an identical copy of the list" do
      @list.clone.to_a.should eql @list.to_a
    end
  end

  describe "List#dup" do
    it "returns a List" do
      @list.dup.should be_a List
    end

    it "returns an identical copy of the list" do
      @list.dup.to_a.should eql @list.to_a
    end
  end

  describe "List#freeze" do
    it "freezes the list" do
      @list.should_not be_frozen
      @list.freeze
      @list.should be_frozen
    end

    it "returns self" do
      @list.freeze.should equal @list
    end
  end

  describe "List#inspect" do
    it "returns a String" do
      @list.inspect.should be_a String
    end
  end
end
