package io.github.judooochp.passage;

import android.graphics.Color;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.RelativeLayout;
import android.widget.TextView;

import org.libsdl.app.R;
import org.libsdl.app.SDLActivity;

/**
 * SDL's activity plus a credits screen shown over the game at every launch.
 * Tapping the credits dismisses them (the link opens the browser); the game's
 * title screen is underneath, waiting for the next tap.
 */
public class PassageActivity extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (mBrokenLibraries || mLayout == null) {
            return;  // SDL is showing its own error dialog
        }
        showCredits();
    }

    private void showCredits() {
        final TextView credits = new TextView(this);
        credits.setText(R.string.credits);
        credits.setAutoLinkMask(Linkify.WEB_URLS);
        credits.setMovementMethod(LinkMovementMethod.getInstance());
        credits.setLinkTextColor(0xFFE8D36A);
        credits.setTextColor(0xFFBBBBBB);
        credits.setTextSize(TypedValue.COMPLEX_UNIT_SP, 15);
        credits.setGravity(Gravity.CENTER);
        credits.setBackgroundColor(Color.BLACK);
        int pad = (int) (24 * getResources().getDisplayMetrics().density);
        credits.setPadding(pad, pad, pad, pad);

        credits.setOnClickListener(v -> mLayout.removeView(credits));

        mLayout.addView(credits, new RelativeLayout.LayoutParams(
                RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT));
    }
}
