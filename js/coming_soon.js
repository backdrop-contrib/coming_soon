(function ($) {
  Backdrop.behaviors.comingSoonRangePreview = {
    attach: function (context) {
      $("input[type='range']", context).once('comingSoonRangePreview').each(function () {
        var $input = $(this);
        var name = $input.attr("name");
        var $output = $(".range-value[data-for='" + name + "']", context);

        if ($output.length) {
          $output.text($input.val());
          $input.on("input", function () {
            $output.text($(this).val());
          });
        }
      });
    }
  };
})(jQuery);
