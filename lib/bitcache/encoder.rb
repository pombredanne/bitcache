module Bitcache
  ##
  class Encoder
    ##
    # Returns an encoder class identified by `name`.
    #
    # @param  [Integer, Symbol] base
    # @return [Class]
    def self.for(base)
      case base
        when 16, :base16 then Base16
        when 62, :base62 then Base62
        when 94, :base94 then Base94
      end
    end

    ##
    # Returns an encoder class identified by `name`.
    #
    # @param  [Integer, Symbol] base
    # @return [Class]
    def self.[](base)
      self.for(base)
    end

    private_class_method :new

    ##
    class Base < Encoder
      ##
      # Returns the numeric base for this encoder class.
      #
      # @return [Integer]
      def self.base
        digits.size
      end

      ##
      # Returns the array of digits for this encoder class.
      #
      # @return [Array<Integer>]
      def self.digits
        const_get(:DIGITS)
      end

      ##
      # Returns a regular expression matching the digits of this encoder
      # class.
      #
      # @return [Regexp
      def self.regexp
        const_defined?(:REGEXP) ? const_get(:REGEXP) :
          /^(?:#{digits.map { |digit| Regexp.quote(digit.chr) }.join('|')})+$/
      end

      ##
      # Returns `true` if `id` matches the digits in this encoder class.
      #
      # @example
      #   Base16 === "deadbeef"  #=> true
      #   Base16 === "foobar"    #=> false
      #
      # @param  [String, #to_s] id
      # @return [Boolean]
      def self.===(id)
        self.regexp === id.to_s
      end

      ##
      # Encodes a number using this encoder class.
      #
      # @param  [Integer] number
      # @return [String]
      def self.encode(number)
        result = []
        while number > 0
          number, digit = number.divmod(base)
          result.unshift(digits[digit].chr)
        end
        result.empty? ? digits.first.chr : result.join('')
      end

      ##
      # Decodes a string using this encoder class.
      #
      # @param  [String] string
      # @return [Integer]
      def self.decode(string)
        result, index = 0, 0
        string.reverse.each_byte do |char|
          result += digits.index(char.ord) * (base ** index)
          index  += 1
        end
        result
      end
    end # Base

    ##
    # Base-16 (hexadecimal) encoder.
    #
    # @see Base
    # @see http://en.wikipedia.org/wiki/Hexadecimal
    class Base16 < Base
      DIGITS = ((?0..?9).to_a + (?a..?f).to_a).map(&:ord)
      REGEXP = /^[0-9a-f]+$/
    end # Base16

    ##
    # Base-62 encoder.
    #
    # @see Base
    # @see http://en.wikipedia.org/wiki/Base_62
    class Base62 < Base
      DIGITS = ((?0..?9).to_a + (?A..?Z).to_a + (?a..?z).to_a).map(&:ord)
      REGEXP = /^[0-9A-Za-z]+$/
    end # Base62

    ##
    # Base-94 encoder.
    #
    # @see Base
    class Base94 < Base
      DIGITS = ((33..126).to_a).map(&:ord)
      REGEXP = /^(?:#{DIGITS.map { |digit| Regexp.quote(digit.chr) }.join('|')})+$/
    end # Base94
  end # Encoder
end # Bitcache
